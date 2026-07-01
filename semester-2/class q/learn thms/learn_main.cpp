#include <cstdint>       // std::uint64_t
#include <iostream>      // std::cout
#include <memory>        // std::unique_ptr, std::make_unique
#include <stdexcept>     // std::runtime_error
#include <string>        // std::string, std::to_string
#include <unordered_map> // std::unordered_map
#include <utility>       // std::move
#include <vector>        // std::vector

enum class PaymentMethod { Card, BankTransfer, Crypto }; // Какие способы оплаты поддерживаем.
enum class PaymentStatus { Approved, Declined, Error };  // Возможные результаты обработки платежа.

struct PaymentRequest {             // Входные данные для сервиса оплаты.
    std::string order_id;           // Идентификатор заказа.
    std::string user_id;            // Идентификатор пользователя.
    double amount_usd;              // Сумма в USD.
    PaymentMethod method;           // Выбранный способ оплаты.
    std::string idempotency_key;    // Ключ идемпотентности (защита от повторного списания).
};                                  // Конец структуры запроса.

struct PaymentResult {              // Выходные данные после обработки.
    PaymentStatus status;           // Статус: успех/отказ/ошибка.
    std::string provider_name;      // Какой провайдер обработал (или кто отказал).
    std::string provider_txn_id;    // ID транзакции у провайдера.
    std::string message;            // Человекочитаемое сообщение.
};                                  // Конец структуры результата.

struct RiskDecision {               // Результат работы антифрода.
    bool allowed;                   // Разрешено ли проводить платеж.
    std::string reason;             // Причина (например, "User is blocked").
};                                  // Конец структуры risk-решения.

class IRiskPolicy {                                                     // Интерфейс risk-политики.
public:                                                                 // Публичная часть интерфейса.
    virtual ~IRiskPolicy() = default;                                   // Виртуальный деструктор обязателен для полиморфизма.
    virtual RiskDecision evaluate(const PaymentRequest& request) const = 0; // Чисто виртуальный метод: оценить риск.
};                                                                      // Конец интерфейса IRiskPolicy.

class AmountLimitRiskPolicy : public IRiskPolicy {               // Политика: ограничение максимальной суммы.
private:                                                         // Приватные данные класса.
    double max_amount_usd;                                       // Лимит суммы.

public:                                                          // Публичные методы класса.
    explicit AmountLimitRiskPolicy(double max_amount)            // Конструктор принимает лимит.
        : max_amount_usd(max_amount) {}                          // Сохраняем лимит.

    RiskDecision evaluate(const PaymentRequest& request) const override { // Переопределяем интерфейсный метод.
        if (request.amount_usd > max_amount_usd) {                     // Если сумма больше лимита...
            return {false, "Amount is above risk limit"};              // ...то блокируем платеж.
        }                                                               // Конец if.
        return {true, "OK"};                                            // Иначе пропускаем.
    }                                                                   // Конец evaluate.
};                                                                      // Конец AmountLimitRiskPolicy.

class BlockedUserRiskPolicy : public IRiskPolicy {          // Политика: блокировка конкретных пользователей.
private:                                                    // Приватные данные класса.
    std::unordered_map<std::string, bool> blocked_users;   // Быстрый поиск: user_id -> заблокирован/нет.

public:                                                     // Публичные методы класса.
    explicit BlockedUserRiskPolicy(std::vector<std::string> blocked) { // Конструктор получает список заблокированных.
        for (const auto& id : blocked) {                                // Проходим все id из входного списка.
            blocked_users[id] = true;                                   // Помечаем каждого как заблокированного.
        }                                                               // Конец цикла.
    }                                                                   // Конец конструктора.

    RiskDecision evaluate(const PaymentRequest& request) const override { // Проверка пользователя.
        auto it = blocked_users.find(request.user_id);                     // Ищем user_id в hash-таблице.
        if (it != blocked_users.end() && it->second) {                     // Если нашли и он заблокирован...
            return {false, "User is blocked"};                             // ...запрещаем платеж.
        }                                                                  // Конец if.
        return {true, "OK"};                                               // Иначе разрешаем.
    }                                                                      // Конец evaluate.
};                                                                         // Конец BlockedUserRiskPolicy.

class CompositeRiskPolicy : public IRiskPolicy {                    // Композит: объединяет несколько политик в pipeline.
private:                                                            // Приватные данные класса.
    std::vector<std::unique_ptr<IRiskPolicy>> policies;             // Набор полиморфных риск-правил.

public:                                                             // Публичные методы класса.
    void add_policy(std::unique_ptr<IRiskPolicy> policy) {          // Добавляем новую политику в pipeline.
        policies.push_back(std::move(policy));                      // Перемещаем unique_ptr в вектор.
    }                                                               // Конец add_policy.

    RiskDecision evaluate(const PaymentRequest& request) const override { // Прогоняем запрос по всем политикам.
        for (const auto& policy : policies) {                            // Идем по правилам по порядку.
            RiskDecision decision = policy->evaluate(request);           // Вызываем полиморфно evaluate().
            if (!decision.allowed) {                                     // Если правило отклонило...
                return decision;                                         // ...сразу возвращаем причину.
            }                                                            // Конец if.
        }                                                                // Конец цикла.
        return {true, "OK"};                                             // Если ни одно правило не отклонило.
    }                                                                    // Конец evaluate.
};                                                                       // Конец CompositeRiskPolicy.

class IAuditLogger {                                      // Интерфейс логгера аудита.
public:                                                   // Публичная часть интерфейса.
    virtual ~IAuditLogger() = default;                   // Виртуальный деструктор.
    virtual void log(const std::string& message) const = 0; // Чисто виртуальный метод логирования.
};                                                        // Конец интерфейса IAuditLogger.

class ConsoleAuditLogger : public IAuditLogger {         // Реализация логгера в консоль.
public:                                                   // Публичные методы.
    void log(const std::string& message) const override { // Переопределяем log().
        std::cout << "[AUDIT] " << message << '\n';       // Печатаем сообщение аудита.
    }                                                      // Конец log().
};                                                         // Конец ConsoleAuditLogger.

class IPaymentGateway {                                              // Интерфейс провайдера оплаты.
public:                                                              // Публичная часть интерфейса.
    virtual ~IPaymentGateway() = default;                            // Виртуальный деструктор.
    virtual PaymentResult charge(const PaymentRequest& request) = 0; // Чисто виртуальный метод списания.
};                                                                   // Конец интерфейса IPaymentGateway.

class BaseGateway : public IPaymentGateway {                          // Базовый класс для конкретных gateway.
protected:                                                            // Protected для наследников.
    std::string provider_name;                                        // Имя провайдера.
    std::uint64_t sequence = 1000;                                    // Простой счетчик транзакций.

    std::string next_txn_id() {                                       // Генерация следующего id транзакции.
        ++sequence;                                                    // Увеличиваем последовательность.
        return provider_name + "-TXN-" + std::to_string(sequence);    // Собираем строковый id.
    }                                                                  // Конец next_txn_id().

public:                                                                // Публичные методы.
    explicit BaseGateway(std::string name) : provider_name(std::move(name)) {} // Конструктор сохраняет имя провайдера.
    virtual ~BaseGateway() = default;                                  // Виртуальный деструктор.
};                                                                     // Конец BaseGateway.

class CardGateway : public BaseGateway {                               // Провайдер оплаты картой.
public:                                                                // Публичные методы.
    CardGateway() : BaseGateway("CardGateway") {}                      // Задаем имя провайдера.

    PaymentResult charge(const PaymentRequest& request) override {     // Реализация списания для карт.
        if (request.amount_usd > 1500.0) {                             // Условная бизнес-логика: для больших сумм отклоняем.
            return {PaymentStatus::Declined, provider_name, next_txn_id(), "3DS authentication required"}; // Возвращаем отказ.
        }                                                               // Конец if.
        return {PaymentStatus::Approved, provider_name, next_txn_id(), "Card charged"}; // Иначе одобряем.
    }                                                                   // Конец charge().
};                                                                      // Конец CardGateway.

class BankTransferGateway : public BaseGateway {                       // Провайдер банковского перевода.
public:                                                                // Публичные методы.
    BankTransferGateway() : BaseGateway("BankGateway") {}              // Задаем имя провайдера.

    PaymentResult charge(const PaymentRequest& request) override {     // Реализация списания для перевода.
        if (request.amount_usd < 10.0) {                               // Условная бизнес-логика: слишком маленькую сумму отклоняем.
            return {PaymentStatus::Declined, provider_name, next_txn_id(), "Amount is too small for transfer"}; // Возвращаем отказ.
        }                                                               // Конец if.
        return {PaymentStatus::Approved, provider_name, next_txn_id(), "Transfer accepted"}; // Иначе одобряем.
    }                                                                   // Конец charge().
};                                                                      // Конец BankTransferGateway.

class CryptoGateway : public BaseGateway {                             // Провайдер крипто-оплаты.
public:                                                                // Публичные методы.
    CryptoGateway() : BaseGateway("CryptoGateway") {}                  // Задаем имя провайдера.

    PaymentResult charge(const PaymentRequest& request) override {     // Реализация списания для crypto.
        if (request.amount_usd > 2000.0) {                             // Условная ситуация нестабильности провайдера.
            throw std::runtime_error("Provider timeout");              // Показываем исключение как реальную ошибку внешнего сервиса.
        }                                                               // Конец if.
        return {PaymentStatus::Approved, provider_name, next_txn_id(), "On-chain payment accepted"}; // Иначе успех.
    }                                                                   // Конец charge().
};                                                                      // Конец CryptoGateway.

class PaymentGatewayFactory {                                           // Фабрика: создает нужный gateway по методу оплаты.
public:                                                                 // Публичные методы.
    static std::unique_ptr<IPaymentGateway> create(PaymentMethod method) { // Статический метод для выбора провайдера.
        switch (method) {                                               // Выбираем реализацию по enum.
            case PaymentMethod::Card:                                   // Если карта...
                return std::make_unique<CardGateway>();                 // ...создаем CardGateway.
            case PaymentMethod::BankTransfer:                           // Если банковский перевод...
                return std::make_unique<BankTransferGateway>();         // ...создаем BankTransferGateway.
            case PaymentMethod::Crypto:                                 // Если крипта...
                return std::make_unique<CryptoGateway>();               // ...создаем CryptoGateway.
        }                                                               // Конец switch.
        throw std::runtime_error("Unknown payment method");             // Защита на случай неизвестного enum.
    }                                                                   // Конец create().
};                                                                      // Конец PaymentGatewayFactory.

class PaymentService {                                                   // Основной сервис (оркестратор бизнес-процесса).
private:                                                                 // Приватные поля сервиса.
    const IRiskPolicy& risk_policy;                                      // Зависимость от интерфейса risk-движка.
    const IAuditLogger& audit_logger;                                    // Зависимость от интерфейса аудита.
    std::unordered_map<std::string, PaymentResult> processed_by_idempotency_key; // Память обработанных idempotency ключей.

public:                                                                  // Публичные методы сервиса.
    PaymentService(const IRiskPolicy& risk, const IAuditLogger& logger)  // Конструктор получает зависимости извне.
        : risk_policy(risk), audit_logger(logger) {}                     // Инициализация ссылок на зависимости.

    PaymentResult process(const PaymentRequest& request) {               // Главный метод обработки платежа.
        auto existing = processed_by_idempotency_key.find(request.idempotency_key); // Ищем, не обрабатывали ли уже такой ключ.
        if (existing != processed_by_idempotency_key.end()) {            // Если уже обрабатывали...
            audit_logger.log("Idempotency hit for key=" + request.idempotency_key); // Логируем повтор.
            return existing->second;                                      // Возвращаем прежний результат без повторного списания.
        }                                                                 // Конец if.

        RiskDecision risk = risk_policy.evaluate(request);                // Запускаем risk-проверку (полиморфно).
        if (!risk.allowed) {                                              // Если риск не прошел...
            PaymentResult declined{                                       // Формируем отказ.
                PaymentStatus::Declined,                                  // Статус отказ.
                "RiskEngine",                                             // Кто отказал.
                "RISK-0",                                                 // Технический id решения.
                "Declined by risk: " + risk.reason                        // Текст причины.
            };                                                            // Конец инициализации declined.
            processed_by_idempotency_key[request.idempotency_key] = declined; // Запоминаем решение по ключу.
            audit_logger.log("Risk declined order=" + request.order_id + ", reason=" + risk.reason); // Пишем аудит.
            return declined;                                              // Возвращаем отказ.
        }                                                                 // Конец if (risk).

        try {                                                             // Блок для обработки ошибок внешнего gateway.
            std::unique_ptr<IPaymentGateway> gateway = PaymentGatewayFactory::create(request.method); // Выбираем провайдера через фабрику.
            PaymentResult result = gateway->charge(request);              // Выполняем списание (полиморфный вызов).
            processed_by_idempotency_key[request.idempotency_key] = result; // Запоминаем итог по idempotency ключу.
            audit_logger.log("Order=" + request.order_id + ", provider=" + result.provider_name +
                             ", status=" + to_string(result.status));     // Логируем итог.
            return result;                                                // Возвращаем результат.
        } catch (const std::exception& ex) {                              // Ловим любые std::exception от gateway.
            PaymentResult error{                                          // Формируем техническую ошибку.
                PaymentStatus::Error,                                     // Статус Error.
                "GatewayRuntime",                                         // Источник ошибки.
                "ERR-1",                                                  // Технический код ошибки.
                std::string("Gateway exception: ") + ex.what()            // Сообщение из исключения.
            };                                                            // Конец инициализации error.
            processed_by_idempotency_key[request.idempotency_key] = error; // Тоже запоминаем по ключу (важно для идемпотентности).
            audit_logger.log("Order=" + request.order_id + ", gateway exception"); // Логируем исключение.
            return error;                                                 // Возвращаем ошибку.
        }                                                                 // Конец try/catch.
    }                                                                     // Конец process().

    static std::string to_string(PaymentStatus status) {                  // Утилита: enum -> строка.
        switch (status) {                                                 // Выбираем текст по статусу.
            case PaymentStatus::Approved:                                 // Ветка успеха.
                return "Approved";                                        // Текст для успеха.
            case PaymentStatus::Declined:                                 // Ветка отказа.
                return "Declined";                                        // Текст для отказа.
            case PaymentStatus::Error:                                    // Ветка ошибки.
                return "Error";                                           // Текст для ошибки.
        }                                                                 // Конец switch.
        return "Unknown";                                                 // Защита на случай нового статуса.
    }                                                                     // Конец to_string().
};                                                                        // Конец PaymentService.

void print_result(const PaymentResult& result) {                          // Вспомогательная печать результата.
    std::cout << "Result: status=" << PaymentService::to_string(result.status) // Печать статуса.
              << ", provider=" << result.provider_name                    // Печать имени провайдера.
              << ", txn=" << result.provider_txn_id                       // Печать id транзакции.
              << ", msg=" << result.message << '\n';                      // Печать сообщения.
}                                                                         // Конец print_result().

int main() {                                                              // Точка входа в программу.
    CompositeRiskPolicy risk_pipeline;                                    // Создаем pipeline риск-правил.
    risk_pipeline.add_policy(std::make_unique<AmountLimitRiskPolicy>(5000.0)); // Добавляем лимит суммы.
    risk_pipeline.add_policy(                                             // Добавляем правило блокировки пользователей.
        std::make_unique<BlockedUserRiskPolicy>(
            std::vector<std::string>{"user_blacklisted_42", "fraud_user"})); // Передаем список заблокированных.

    ConsoleAuditLogger logger;                                            // Создаем logger.
    PaymentService payment_service(risk_pipeline, logger);                // Внедряем зависимости в сервис.

    std::vector<PaymentRequest> demo_requests{                            // Тестовые запросы.
        {"order-100", "user_1", 120.0, PaymentMethod::Card, "idem-100"},             // Нормальная оплата картой.
        {"order-101", "fraud_user", 20.0, PaymentMethod::BankTransfer, "idem-101"},  // Должно отклониться риск-правилом.
        {"order-102", "user_2", 2500.0, PaymentMethod::Crypto, "idem-102"},           // Должно упасть в exception gateway.
        {"order-100-repeat", "user_1", 120.0, PaymentMethod::Card, "idem-100"}        // Повтор с тем же idempotency_key.
    };                                                                    // Конец массива тестов.

    for (const auto& request : demo_requests) {                          // Обрабатываем все тестовые запросы.
        std::cout << "\nProcess order=" << request.order_id << '\n';     // Печатаем текущий заказ.
        PaymentResult result = payment_service.process(request);          // Вызываем основной метод сервиса.
        print_result(result);                                             // Печатаем итог.
    }                                                                     // Конец цикла.

    return 0;                                                             // Успешное завершение программы.
}                                                                         // Конец main().

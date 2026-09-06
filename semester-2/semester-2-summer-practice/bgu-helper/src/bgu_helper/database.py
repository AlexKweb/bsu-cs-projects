import psycopg
import requests
from datetime import datetime
from bgu_helper.logger import logger


DATABASE = "dbname=bgu_helper"


def create_table():
    try:
        with psycopg.connect(DATABASE) as connection:
            connection.execute("""
            CREATE TABLE IF NOT EXISTS news (
                id SERIAL PRIMARY KEY,
                title TEXT,
                date TEXT,
                href TEXT
            )
            """)

            connection.commit()

        logger.info("Таблица news готова к работе")

    except Exception as e:
        logger.error(f"Ошибка работы с базой данных: {e}")


months = {
    "января": 1,
    "февраля": 2,
    "марта": 3,
    "апреля": 4,
    "мая": 5,
    "июня": 6,
    "июля": 7,
    "августа": 8,
    "сентября": 9,
    "октября": 10,
    "ноября": 11,
    "декабря": 12
}


def parse_news_date(date_string):
    day, month, year = date_string.split()

    return datetime(
        int(year),
        months[month],
        int(day)
    )


async def load_news() -> str:
    rows_for_news = ""
    try:
        async with await psycopg.AsyncConnection.connect(DATABASE) as connection:

            cursor = await connection.execute(
                "SELECT date, title, href FROM news"
            )

            news = await cursor.fetchall()

            news = sorted(
                news,
                key=lambda item: parse_news_date(item[0]),
                reverse=True
            )

            logger.info(f"Из базы загружено новостей: {len(news)}")

            for item in news:
                rows_for_news += f"""
                    <tr>
                        <td>{item[0]}</td>
                        <td>
                            <a href="https://bsu.by{item[2]}">{item[1]}</a>
                        </td>
                    </tr>
                    """
        return rows_for_news
    except Exception as e:
        logger.error(f"Ошибка при загрузке новостей: {e}")
        return "<tr><td colspan='2'>Ошибка при загрузке новостей</td></tr>"


def get_news():
    try:
        response = requests.get(
            "https://bsu.by/news/?bxajaxid=10ba193de64d376b63f47f6da3252473"
        )

        response.raise_for_status()

        news = response.json()["data"]["data"]["items"]

        logger.info(f"С сайта БГУ получено новостей: {len(news)}")

        connection = psycopg.connect(DATABASE)

        added = 0
        skipped = 0

        for item in news:
            old_news = connection.execute(
                "SELECT id FROM news WHERE href = %s",
                (item["href"],)
            ).fetchone()

            if old_news is None:
                connection.execute(
                    "INSERT INTO news (title, date, href) VALUES (%s, %s, %s)",
                    (
                        item["title"],
                        item["date"],
                        item["href"]
                    )
                )

                added += 1

            else:
                skipped += 1

        connection.commit()
        connection.close()

        logger.info(f"Добавлено новых новостей: {added}")
        logger.info(f"Уже были в базе: {skipped}")

        with open("src/bgu_helper/index.html", encoding="utf-8") as f:
            lines = f.readlines()

        with open("src/bgu_helper/index.html", "w", encoding="utf-8") as f:
            for line in lines:
                if "<!-- LAST_UPDATE -->" in line:
                    line = (
                        "            <p><!-- LAST_UPDATE -->"
                        f"Последнее обновление: "
                        f"{datetime.now().strftime('%d.%m.%Y %H:%M')}"
                        "</p>\n"
                    )

                f.write(line)

        return
    except Exception as e:
        logger.error(f"Ошибка получения новостей из базы: {e}")

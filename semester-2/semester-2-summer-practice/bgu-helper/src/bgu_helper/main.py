from fastapi import FastAPI
from fastapi.responses import RedirectResponse, HTMLResponse
from datetime import datetime
import requests
import uvicorn

from bgu_helper.database import load_news, create_table, get_news
from bgu_helper.logger import logger
from bgu_helper.parser import get_schedule, load_schedule


app = FastAPI()

create_table()

schedule = None


@app.get("/")
def home():
    try:
        logger.info("Открыта главная страница")

        rows_for_news = load_news()

        # ------------------------------------

        if schedule:
            rows_for_schedule = load_schedule(schedule)
        else:
            logger.info("Расписание не загружено")
            rows_for_schedule = """
                <tr>
                    <td colspan="2">Расписание пока не загружено</td>
                </tr>"""

        # ------------------------------------
        try:
            response = requests.get(
                "https://api.open-meteo.com/v1/forecast",
                params={
                    "latitude": 53.9,
                    "longitude": 27.5667,
                    "current": "temperature_2m"
                }
            )
            response.raise_for_status()
            weather = response.json()["current"]["temperature_2m"]
            logger.info(f"Получена погода: {weather}°C")
        except Exception as e:
            logger.error(f"Ошибка получения погоды: {e}")
            weather = "Не удалось получить погоду"

        with open("src/bgu_helper/index.html", encoding="utf-8") as f:
            html = f.read()
        html = html.replace("<!-- NEWS -->", rows_for_news)
        html = html.replace("<!-- SCHEDULE -->", rows_for_schedule)
        html = html.replace("<!-- WEATHER -->", str(weather))

        return HTMLResponse(html)

    except Exception as e:
        logger.error(f"Ошибка главной страницы: {e}")

        return HTMLResponse(
            "<h1>Произошла ошибка</h1>",
            status_code=500
        )


@app.post("/update-schedule")
def update_schedule():
    try:
        logger.info("Начато обновление расписания")
        global schedule
        schedule = get_schedule()
        logger.info("Обновление расписания завершено")
        return RedirectResponse("/", status_code=303)
    except Exception as e:
        logger.error(f"Ошибка обновления расписания: {e}")
        return RedirectResponse("/", status_code=303)


@app.post("/update-news")
def update_news():
    try:
        logger.info("Начато обновление новостей")
        get_news()
        logger.info("Обновление новостей завершено")

    except Exception as e:
        logger.error(f"Ошибка обновления новостей: {e}")

    return RedirectResponse("/", status_code=303)


if __name__ == "__main__":
    logger.info("Сервер BGU Helper запущен")

    uvicorn.run(
        "main:app",
        reload=True
    )

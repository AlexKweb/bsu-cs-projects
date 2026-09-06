from fastapi import FastAPI
from fastapi.responses import RedirectResponse, HTMLResponse
from datetime import datetime
import requests
import uvicorn
import asyncio


from bgu_helper.database import load_news, create_table, get_news
from bgu_helper.logger import logger
from bgu_helper.parser import get_schedule, load_schedule, get_weather

app = FastAPI()

create_table()

schedule = None


@app.get("/")
async def home():
    try:
        logger.info("Открыта главная страница")

        weather_task = asyncio.create_task(get_weather())
        news_task = asyncio.create_task(load_news())

        weather = await weather_task
        rows_for_news = await news_task
        #
        if schedule:
            rows_for_schedule = load_schedule(schedule)
        else:
            logger.info("Расписание не загружено")
            rows_for_schedule = """
                <tr>
                    <td colspan="2">Расписание пока не загружено</td>
                </tr>"""

        with open("src/bgu_helper/index.html", encoding="utf-8") as f:
            html = f.read()

        html = html.replace("<!-- WEATHER -->", str(weather))
        html = html.replace("<!-- SCHEDULE -->", rows_for_schedule)
        html = html.replace("<!-- NEWS -->", rows_for_news)

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

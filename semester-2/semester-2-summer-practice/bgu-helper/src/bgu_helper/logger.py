import datetime


class Logger:
    def __init__(self):
        self.file_path = "logs/app.log"

    def info(self, message: str):
        with open(self.file_path, "a", encoding="utf-8") as f:
            f.write(
                f"{datetime.datetime.now()} | INFO | {message}\n"
            )

    def error(self, message: str):
        with open(self.file_path, "a", encoding="utf-8") as f:
            f.write(
                f"{datetime.datetime.now()} | ERROR | {message}\n"
            )


logger = Logger()

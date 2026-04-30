from fastapi import FastAPI
from fastapi.responses import JSONResponse

app = FastAPI()

@app.get("/products")
def get_products():
    return [
        {"id": 1, "name": "Shoes"},
        {"id": 2, "name": "Bag"}
    ]

# ---------------------------

from fastapi import FastAPI
from fastapi.responses import JSONResponse

app = FastAPI()

@app.get("/health")
def health_check():
    return JSONResponse(
        content={"status": "ok"},
        status_code=200
    )
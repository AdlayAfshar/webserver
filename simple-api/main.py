# pip install fastapi uvicorn

from fastapi import FastAPI

app = FastAPI()

@app.get("/")
def home():
    return {"message": "Hello World"}

# http://127.0.0.1:8000

@app.get("/products")
def get_products():
    return [
        {"id": 1, "name": "Shoes"},
        {"id": 2, "name": "Bag"}
    ]

# http://127.0.0.1:8000/products




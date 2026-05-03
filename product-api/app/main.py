from fastapi import FastAPI

from app.database import Base, engine
from app.routers import product

Base.metadata.create_all(bind=engine)

app = FastAPI(
    title="Shop API",
    description="FastAPI + PostgreSQL + Docker CRUD API",
    version="1.0.0"
)

app.include_router(product.router)


@app.get("/")
def home():
    return {"message": "Shop API is running"}
from fastapi import FastAPI, Depends
from sqlalchemy.orm import Session
from database import SessionLocal
import models
import schemas

app = FastAPI()

# Base.metadata.create_all(bind=engine)

def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()

# ➕ Create
@app.post("/products")
def create_product(product: schemas.ProductCreate, db: Session = Depends(get_db)):
    new_product = models.Product(
        name=product.name,
        price=product.price
    )

    db.add(new_product)
    db.commit()
    db.refresh(new_product)

    return new_product

# 📄 Read (list)
# @app.get("/products")
# def get_products(db: Session = Depends(get_db)):
#     return db.query(models.Product).all()
@app.get("/products")
def get_products(
    skip: int = 0,
    limit: int = 10,
    min_price: int = 0,
    name: str = "",
    db: Session = Depends(get_db)
):
    query = db.query(models.Product)

    if min_price:
        query = query.filter(models.Product.price >= min_price)

    if name:
        query = query.filter(models.Product.name.contains(name))

    products = query.offset(skip).limit(limit).all()

    return products

# 🔍 Read (single)
@app.get("/products/{id}")
def get_product(id: int, db: Session = Depends(get_db)):
    return db.query(models.Product).filter(models.Product.id == id).first()

# ✏️ Update
@app.put("/products/{id}")
def update_product(id: int, product: schemas.ProductCreate, db: Session = Depends(get_db)):
    db_product = db.query(models.Product).filter(models.Product.id == id).first()

    if not db_product:
        return {"error": "not found"}

    db_product.name = product.name
    db_product.price = product.price

    db.commit()
    db.refresh(db_product)

    return db_product

# ❌ Delete
@app.delete("/products/{id}")
def delete_product(id: int, db: Session = Depends(get_db)):
    db_product = db.query(models.Product).filter(models.Product.id == id).first()

    if not db_product:
        return {"error": "not found"}

    db.delete(db_product)
    db.commit()

    return {"message": "deleted"}


# uvicorn main:app --reload

# http://127.0.0.1:8000/docs
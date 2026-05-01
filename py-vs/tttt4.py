from fastapi import FastAPI, Depends
from fastapi.responses import JSONResponse
from sqlalchemy.orm import Session

from pydantic import BaseModel

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker

from sqlalchemy import Column, Integer, String
from sqlalchemy.orm import declarative_base

app = FastAPI()


# ---------- Schemas ----------
# برای ساخت محصول (ورودی)
class ProductCreate(BaseModel):
    name: str
    description: str
    price: int
    in_stock: int


# برای آپدیت (فعلاً مثل Create؛ بعداً می‌تونیم optional کنیم)
class ProductUpdate(BaseModel):
    name: str
    description: str
    price: int
    in_stock: int


# برای خروجی (response)
class Product(BaseModel):
    id: int
    name: str
    description: str
    price: int
    in_stock: int

    class Config:
        orm_mode = True

# * ProductCreate → فقط برای ورودی
# * ProductResponse → فقط برای خروجی

# و این خط:
# from_attributes = True
# یعنی:
# حتی اگر خروجی‌ات object باشه (مثلاً ORM مثل SQLAlchemy)،
# باز هم بتونم ازش دیتا بخونم

# ---------- Databsae ----------

DATABASE_URL = "postgresql://postgres:1234@localhost:5432/shop"

engine = create_engine(DATABASE_URL)

SessionLocal = sessionmaker(bind=engine)

Base = declarative_base()

# ---------- Model ----------
class Product(Base):
    __tablename__ = "products"

    id = Column(Integer, primary_key=True, index=True)
    name = Column(String)
    description = Column(String)
    price = Column(Integer)
    in_stock = Column(Integer)

# # --------------------------
Base.metadata.create_all(bind=engine)

def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()
# ---------- Home ----------
@app.get("/")
def home():
    return {"message": "Hello World"}

@app.get("/health")
def health_check():
    return JSONResponse(
        content={"status": "ok"},
        status_code=200
    )

# ➕ Create
@app.post("/products")
def create_product(product: ProductCreate, db: Session = Depends(get_db)):
    new_product = Product(
        name=product.name,
        description=product.description,
        price=product.price,
        in_stock=product.in_stock
    )

    db.add(new_product)
    db.commit()
    db.refresh(new_product)

    return new_product

# 📄 Read (list)
@app.get("/products")
def get_products(db: Session = Depends(get_db)):
    return db.query(Product).all()

# 🔍 Read (single)
@app.get("/products/{id}")
def get_product(id: int, db: Session = Depends(get_db)):
    return db.query(Product).filter(Product.id == id).first()

# ✏️ Update
@app.put("/products/{id}")
def update_product(id: int, product: ProductCreate, db: Session = Depends(get_db)):
    db_product = db.query(Product).filter(Product.id == id).first()

    if not db_product:
        return {"error": "not found"}

    db_product.name = product.name
    db_product.description = product.description
    db_product.price = product.price
    db_product.in_stock = product.in_stock

    db.commit()
    db.refresh(db_product)

    return db_product

# ❌ Delete
@app.delete("/products/{id}")
def delete_product(id: int, db: Session = Depends(get_db)):
    db_product = db.query(Product).filter(Product.id == id).first()

    if not db_product:
        return {"error": "not found"}

    db.delete(db_product)
    db.commit()

    return {"message": "deleted"}
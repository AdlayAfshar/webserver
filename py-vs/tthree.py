from fastapi import FastAPI, Depends, HTTPException
from fastapi.responses import JSONResponse
from pydantic import BaseModel

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker, declarative_base

from sqlalchemy import Column, Integer, String

from sqlalchemy.orm import Session

app = FastAPI()


# ---------- Schemas ----------
class ProductCreate(BaseModel):
    name: str
    description: str
    price: int
    in_stock: int

class ProductResponse(BaseModel):
    id: int
    name: str
    description: str
    price: int
    in_stock: int

    class Config:
        from_attributes = True

# ProductCreate → فقط برای ورودی
# ProductResponse → فقط برای خروجی

# و این خط:
# from_attributes = True
# یعنی:
# حتی اگر خروجی‌ات object باشه (مثلاً ORM مثل SQLAlchemy)،
# باز هم بتونم ازش دیتا بخونم

# ---------- Databsae ----------

DATABASE_URL = "sqlite:///./store.db"

engine = create_engine(
    DATABASE_URL,
    connect_args={"check_same_thread": False}
)

SessionLocal = sessionmaker(
    autocommit=False,
    autoflush=False,
    bind=engine
)

Base = declarative_base()
# Base
# همه مدل‌های دیتابیس از این ساخته می‌شوند.

# ---------- Model ----------
class Product(Base):
    __tablename__ = "products"

    id = Column(Integer, primary_key=True, index=True)
    name = Column(String, index=True)
    description = Column(String, index=True)
    price = Column(Integer)
    in_stock = Column(Integer)


# ---------- Crud ----------
def create_product(db: Session, product: ProductCreate):
    db_product = Product(
        name=product.name,
        description=product.description,
        price=product.price,
        in_stock=product.in_stock
    )
    db.add(db_product)
    db.commit()
    db.refresh(db_product)
    return db_product

def get_products(db: Session):
    return db.query(Product).all()

def get_product_by_id(db: Session, product_id: int):
    return db.query(Product).filter(
        Product.id == product_id
    ).first()
# --------------------------
Base.metadata.create_all(bind=engine)
# .این خط می‌گوید:  اگر جدول‌ها هنوز وجود ندارند، بسازشان

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


# ---------- CREATE ----------
@app.post("/products", response_model=ProductResponse)
def create_product(
    product: ProductCreate,
    db: Session = Depends(get_db)
):
    return create_product(db, product)

# curl -X POST http://localhost:8000/products \
# -H "Content-Type: application/json" \
# -d '{"name":"Shoes","description":"Flat","price":80,"in_stock":10}'


# ---------- READ ALL ----------
@app.get("/products", response_model=list[ProductResponse])
def read_products(db: Session = Depends(get_db)):
    return get_products(db)
# curl http://localhost:8000/products

# ---------- READ ONE ----------
@app.get("/products/{product_id}", response_model=ProductResponse)
def read_product(product_id: int, db: Session = Depends(get_db)):
    product = get_product_by_id(db, product_id)
    if product is None:
        raise HTTPException(status_code=404, detail="Product not found")
    return product

# ---------- UPDATE ----------
# @app.put("/products/{id}", response_model=Product)
# def update_product(id: int, product: ProductCreate):
#     for p in products:
#         if p["id"] == id:
#             p["name"] = product.name
#             p["description"] = product.description
#             p["price"] = product.price
#             p["in_stock"] = product.price
#             return p

#     raise HTTPException(status_code=404, detail="Product not found")

# # curl -X PUT http://localhost:8000/products/1 \
# # -H "Content-Type: application/json" \
# # -d '{"name":"New Shoes","price":100}'

# # ---------- DELETE ----------
# @app.delete("/products/{id}")
# def delete_product(id: int):
#     for p in products:
#         if p["id"] == id:
#             products.remove(p)
#             return {"message": "deleted"}

#     raise HTTPException(status_code=404, detail="Product not found")
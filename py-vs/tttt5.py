from fastapi import FastAPI, Depends, HTTPException
from fastapi.responses import JSONResponse

from pydantic import BaseModel
from sqlalchemy import create_engine, Column, Integer, String
from sqlalchemy.orm import sessionmaker, declarative_base, Session

app = FastAPI()

# =========================
# Schemas
# =========================

# ورودی create
class ProductCreate(BaseModel):
    name: str
    description: str
    price: int
    in_stock: int

# ورودی update
class ProductUpdate(BaseModel):
    name: str
    description: str
    price: int
    in_stock: int

# خروجی
class ProductResponse(BaseModel):
    id: int
    name: str
    description: str
    price: int
    in_stock: int

    class Config:
        from_attributes = True


# =========================
# Database (PostgreSQL)
# =========================
#
DATABASE_URL = "postgresql://postgres:1234@localhost:5432/shop"

# DATABASE_URL connection URL
# username = postgres
# password = 1234
# host = localhost
# port = 5432
# database = shop

engine = create_engine(DATABASE_URL)

SessionLocal = sessionmaker(
    autocommit=False,
    autoflush=False,
    bind=engine
)

Base = declarative_base()


# =========================
# Model
# =========================

class Product(Base):
    __tablename__ = "products"

    id = Column(Integer, primary_key=True, index=True)
    name = Column(String)
    description = Column(String)
    price = Column(Integer)
    in_stock = Column(Integer)


# =========================
# CRUD (logic جدا)
# =========================

def create_product(db: Session, product: ProductCreate):
    db_product = Product(**product.dict())
    db.add(db_product)
    db.commit()
    db.refresh(db_product)
    return db_product


def get_products(db: Session):
    return db.query(Product).all()


def get_product_by_id(db: Session, product_id: int):
    return db.query(Product).filter(Product.id == product_id).first()


def update_product(db: Session, product_id: int, product: ProductUpdate):
    db_product = get_product_by_id(db, product_id)

    if not db_product:
        return None

    for key, value in product.dict().items():
        setattr(db_product, key, value)

    db.commit()
    db.refresh(db_product)
    return db_product


def delete_product(db: Session, product_id: int):
    db_product = get_product_by_id(db, product_id)

    if not db_product:
        return None

    db.delete(db_product)
    db.commit()
    return db_product


# =========================
# DB init
# =========================

Base.metadata.create_all(bind=engine)


def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()


# =========================
# Routes
# =========================

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
def create_product_route(
    product: ProductCreate,
    db: Session = Depends(get_db)
):
    return create_product(db, product)


# ---------- READ ALL ----------
@app.get("/products", response_model=list[ProductResponse])
def read_products(db: Session = Depends(get_db)):
    return get_products(db)


# ---------- READ ONE ----------
@app.get("/products/{product_id}", response_model=ProductResponse)
def read_product(product_id: int, db: Session = Depends(get_db)):
    product = get_product_by_id(db, product_id)

    if product is None:
        raise HTTPException(status_code=404, detail="Product not found")

    return product


# ---------- UPDATE ----------
@app.put("/products/{product_id}", response_model=ProductResponse)
def update_product_route(
    product_id: int,
    product: ProductUpdate,
    db: Session = Depends(get_db)
):
    updated = update_product(db, product_id, product)

    if updated is None:
        raise HTTPException(status_code=404, detail="Product not found")

    return updated


# ---------- DELETE ----------
@app.delete("/products/{product_id}")
def delete_product_route(
    product_id: int,
    db: Session = Depends(get_db)
):
    deleted = delete_product(db, product_id)

    if deleted is None:
        raise HTTPException(status_code=404, detail="Product not found")

    return {"message": "deleted"}
from fastapi import APIRouter, Depends, HTTPException, Query
from sqlalchemy.orm import Session

from app.database import get_db
from app import models, schemas

router = APIRouter(
    prefix="/products",
    tags=["Products"]
)


@router.post("/", response_model=schemas.ProductResponse, status_code=201)
def create_product(
    product: schemas.ProductCreate,
    db: Session = Depends(get_db)
):
    new_product = models.Product(
        name=product.name,
        description=product.description,
        price=product.price,
        in_stock=product.in_stock
    )

    db.add(new_product)
    db.commit()
    db.refresh(new_product)

    return new_product

# decorator @router.post() is equivalent to:
# router.add_api_route("/", create_product, response_model=schemas.ProductResponse, status_code=201)

@router.get("/", response_model=list[schemas.ProductResponse])
def list_products(
    skip: int = Query(0, ge=0),
    limit: int = Query(10, ge=1, le=50),
    name: str | None = None,
    min_price: int | None = Query(None, ge=0),
    max_price: int | None = Query(None, ge=0),
    in_stock: int | None = Query(None, ge=0),
    db: Session = Depends(get_db)
):
    query = db.query(models.Product)

    if name:
        query = query.filter(models.Product.name.ilike(f"%{name}%"))

    if min_price is not None:
        query = query.filter(models.Product.price >= min_price)

    if max_price is not None:
        query = query.filter(models.Product.price <= max_price)

    if in_stock is not None:
        query = query.filter(models.Product.in_stock >= in_stock)

    products = query.offset(skip).limit(limit).all()

    return products


@router.get("/{product_id}", response_model=schemas.ProductResponse)
def get_product(
    product_id: int,
    db: Session = Depends(get_db)
):
    product = db.query(models.Product).filter(
        models.Product.id == product_id
    ).first()

    if product is None:
        raise HTTPException(status_code=404, detail="Product not found")

    return product


@router.put("/{product_id}", response_model=schemas.ProductResponse)
def update_product(
    product_id: int,
    updated_product: schemas.ProductUpdate,
    db: Session = Depends(get_db)
):
    product = db.query(models.Product).filter(
        models.Product.id == product_id
    ).first()

    if product is None:
        raise HTTPException(status_code=404, detail="Product not found")

    product.name = updated_product.name
    product.description = updated_product.description
    product.price = updated_product.price
    product.in_stock = updated_product.in_stock

    db.commit()
    db.refresh(product)

    return product


@router.delete("/{product_id}")
def delete_product(
    product_id: int,
    db: Session = Depends(get_db)
):
    product = db.query(models.Product).filter(
        models.Product.id == product_id
    ).first()

    if product is None:
        raise HTTPException(status_code=404, detail="Product not found")

    db.delete(product)
    db.commit()

    return {"message": "Product deleted successfully"}
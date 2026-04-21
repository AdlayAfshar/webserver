# # pip install fastapi uvicorn

# from fastapi import FastAPI
# from pydantic import BaseModel

# app = FastAPI()

# @app.get("/")
# def home():
#     return {"message": "Hello World"}

# # http://127.0.0.1:8000

# @app.get("/products")
# def get_products():
#     return [
#         {"id": 1, "name": "Shoes"},
#         {"id": 2, "name": "Bag"}
#     ]

# # http://127.0.0.1:8000/products

# class Product(BaseModel):
#     name: str
#     price: float

# @app.post("/products")
# def create_product(product: Product):
#     return {
#         "message": "created",
#         "product": product
#     }

# # curl -X POST http://localhost:8000/products \
# #   -H "Content-Type: application/json" \
# #   -d '{"name": "Shoes", "price": 80}'

# //////////////////

# uvicorn main:app --reload

# //////////////////

from fastapi import FastAPI, HTTPException
from pydantic import BaseModel

app = FastAPI()

# ---------- Model ----------
class Product(BaseModel):
    name: str
    description: str
    price: float
    in_stock: int


# ---------- Fake Database ----------
products = []
next_id = 1


# ---------- CREATE ----------
@app.post("/products")
def create_product(product: Product):
    global next_id

    new_product = {
        "id": next_id,
        "name": product.name,
        "description": product.description,
        "price": product.price,
        "in_stock": product.in_stock
    }

    products.append(new_product)
    next_id += 1

    return new_product

# curl -X POST http://localhost:8000/products \
# -H "Content-Type: application/json" \
# -d '{"name":"Shoes","description":"Flat","price":80,"in_stock":10}'


# ---------- READ ALL ----------
@app.get("/products")
def get_products():
    return products

# curl http://localhost:8000/products

# ---------- READ ONE ----------
@app.get("/products/{product_id}")
def get_product(product_id: int):
    for p in products:
        if p["id"] == product_id:
            return p

    raise HTTPException(status_code=404, detail="Product not found")


# ---------- UPDATE ----------
@app.put("/products/{product_id}")
def update_product(product_id: int, updated: Product):
    for p in products:
        if p["id"] == product_id:
            p["name"] = updated.name
            p["description"] = updated.description
            p["price"] = updated.price
            p["in_stock"] = updated.price
            return p

    raise HTTPException(status_code=404, detail="Product not found")

# curl -X PUT http://localhost:8000/products/1 \
# -H "Content-Type: application/json" \
# -d '{"name":"New Shoes","price":100}'

# ---------- DELETE ----------
@app.delete("/products/{product_id}")
def delete_product(product_id: int):
    for i in range(len(products)):
        if products[i]["id"] == product_id:
            deleted = products.pop(i)
            return {"message": "deleted", "product": deleted}

    raise HTTPException(status_code=404, detail="Product not found")

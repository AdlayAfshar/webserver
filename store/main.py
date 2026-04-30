from fastapi import FastAPI
from schemas import ProductCreate, Product

app = FastAPI()

products = []
product_id = 1

@app.post("/products", response_model=Product)
def create_product(product: ProductCreate):
    global product_id

    new_product = {
        "id": product_id,
        "name": product.name,
        "price": product.price
    }

    products.append(new_product)
    product_id += 1

    return new_product

@app.get("/products", operation_id="getProducts", response_model=list[Product])
def get_products():
    return products

@app.get("/products/{id}", response_model=Product)
def get_product(id: int):
    for p in products:
        if p["id"] == id:
            return p
    return {"error": "not found"}

@app.put("/products/{id}", response_model=Product)
def update_product(id: int, product: ProductCreate):
    for p in products:
        if p["id"] == id:
            p["name"] = product.name
            p["price"] = product.price
            return p
    return {"error": "not found"}

@app.delete("/products/{id}")
def delete_product(id: int):
    for p in products:
        if p["id"] == id:
            products.remove(p)
            return {"message": "deleted"}
    return {"error": "not found"}
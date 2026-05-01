from fastapi import FastAPI, HTTPException
from fastapi.responses import JSONResponse
from pydantic import BaseModel

app = FastAPI()

# ---------- Model ----------
class Product(BaseModel):
    name: str
    description: str
    price: float
    in_stock: int

# این خط:
# class Product(BaseModel):
# اینجا تو فقط یک کلاس نساختی.
# تو داری تعریف می‌کنی: بدنه درخواست باید این شکلی باشد
# بدنه درخواست = (body)

# ---------- Fake Database ----------
products = []
next_id = 1

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
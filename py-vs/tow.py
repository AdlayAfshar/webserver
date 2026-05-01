from fastapi import FastAPI, HTTPException
from fastapi.responses import JSONResponse
from pydantic import BaseModel

app = FastAPI()

# ---------- Model ----------
class ProductCreate(BaseModel):
    name: str
    description: str
    price: int
    in_stock: int

class Product(ProductCreate):
    id: int

# ProductCreate → چیزی که کاربر اجازه داره بفرسته
# Product → چیزی که سرور اجازه داره برگردونه

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
@app.post("/products", response_model=Product)
def create_product(product: ProductCreate):
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
@app.get("/products", operation_id="getProducts", response_model=list[Product])
def get_products():
    return products

# curl http://localhost:8000/products

# ---------- READ ONE ----------
@app.get("/products/{id}", response_model=Product)
def get_product(id: int):
    for p in products:
        if p["id"] == id:
            return p
    raise HTTPException(status_code=404, detail="Product not found")

# ---------- UPDATE ----------
@app.put("/products/{id}", response_model=Product)
def update_product(id: int, product: ProductCreate):
    for p in products:
        if p["id"] == id:
            p["name"] = product.name
            p["description"] = product.description
            p["price"] = product.price
            p["in_stock"] = product.price
            return p

    raise HTTPException(status_code=404, detail="Product not found")

# curl -X PUT http://localhost:8000/products/1 \
# -H "Content-Type: application/json" \
# -d '{"name":"New Shoes","price":100}'

# ---------- DELETE ----------
@app.delete("/products/{id}")
def delete_product(id: int):
    for p in products:
        if p["id"] == id:
            products.remove(p)
            return {"message": "deleted"}

    raise HTTPException(status_code=404, detail="Product not found")
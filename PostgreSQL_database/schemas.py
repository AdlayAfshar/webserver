from pydantic import BaseModel

# برای ساخت محصول (ورودی)
class ProductCreate(BaseModel):
    name: str
    price: int


# برای آپدیت (فعلاً مثل Create؛ بعداً می‌تونیم optional کنیم)
class ProductUpdate(BaseModel):
    name: str
    price: int


# برای خروجی (response)
class Product(BaseModel):
    id: int
    name: str
    price: int

    class Config:
        orm_mode = True
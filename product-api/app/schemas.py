from pydantic import BaseModel


class ProductCreate(BaseModel):
    name: str
    description: str
    price: int
    in_stock: int


class ProductUpdate(BaseModel):
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
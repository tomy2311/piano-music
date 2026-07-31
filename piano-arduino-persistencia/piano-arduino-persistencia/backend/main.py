"""
main.py — API FastAPI para persistencia del Piano Arduino
-----------------------------------------------------------
Expone dos endpoints principales:

  POST /notas   -> recibe una nota tocada (desde serial_bridge.py) y la guarda en Supabase
  GET  /notas   -> devuelve el historial de notas guardadas (para mostrarlas en la web)

Requisitos:
    pip install fastapi uvicorn supabase python-dotenv

Uso:
    uvicorn main:app --reload
"""

import os
from datetime import datetime
from typing import List

from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
from supabase import create_client, Client
from dotenv import load_dotenv

# ----------------------------------------------------------
# CONFIGURACIÓN — carga las credenciales desde .env
# ----------------------------------------------------------
load_dotenv()

SUPABASE_URL = os.getenv("SUPABASE_URL")
SUPABASE_KEY = os.getenv("SUPABASE_KEY")

if not SUPABASE_URL or not SUPABASE_KEY:
    raise RuntimeError(
        "Faltan SUPABASE_URL y/o SUPABASE_KEY. Definilas en un archivo .env "
        "(ver .env.example)."
    )

supabase: Client = create_client(SUPABASE_URL, SUPABASE_KEY)

NOMBRE_TABLA = "notas_tocadas"

# ----------------------------------------------------------
# APP FASTAPI
# ----------------------------------------------------------
app = FastAPI(title="API Piano Arduino - Persistencia")

# CORS: permite que el frontend (index.html) le hable a esta API
# desde otro origen (por ejemplo file:// o localhost con otro puerto).
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # En producción, restringir a tu dominio real
    allow_methods=["*"],
    allow_headers=["*"],
)


# ----------------------------------------------------------
# MODELOS (Pydantic) — validan la forma de los datos
# ----------------------------------------------------------
class NotaEntrada(BaseModel):
    nota: str  # "DO", "RE", "MI" o "FA"


class NotaSalida(BaseModel):
    id: int
    nota: str
    fecha: str


# ----------------------------------------------------------
# ENDPOINTS
# ----------------------------------------------------------

@app.get("/")
def raiz():
    return {"mensaje": "API del Piano Arduino corriendo correctamente"}


@app.post("/notas", status_code=201)
def guardar_nota(entrada: NotaEntrada):
    """
    Recibe una nota tocada en el piano físico (enviada por serial_bridge.py)
    y la inserta en la tabla de Supabase.
    """
    notas_validas = {"DO", "RE", "MI", "FA"}
    if entrada.nota not in notas_validas:
        raise HTTPException(status_code=400, detail="Nota inválida")

    resultado = (
        supabase.table(NOMBRE_TABLA)
        .insert({"nota": entrada.nota})
        .execute()
    )

    if not resultado.data:
        raise HTTPException(status_code=500, detail="No se pudo guardar la nota")

    return resultado.data[0]


@app.get("/notas", response_model=List[NotaSalida])
def obtener_notas(limite: int = 50):
    """
    Devuelve el historial de notas tocadas, de la más reciente a la más vieja.
    Este endpoint es el que consulta la web para mostrar la información persistida.
    """
    resultado = (
        supabase.table(NOMBRE_TABLA)
        .select("*")
        .order("fecha", desc=True)
        .limit(limite)
        .execute()
    )
    return resultado.data


@app.get("/notas/resumen")
def resumen_notas():
    """
    Devuelve cuántas veces se tocó cada nota en total.
    Endpoint extra, útil para mostrar estadísticas en la web.
    """
    resultado = supabase.table(NOMBRE_TABLA).select("nota").execute()
    conteo = {"DO": 0, "RE": 0, "MI": 0, "FA": 0}
    for fila in resultado.data:
        if fila["nota"] in conteo:
            conteo[fila["nota"]] += 1
    return conteo

# Piano Arduino — Persistencia con FastAPI + Supabase

Arquitectura:

```
[Arduino físico] --Serial USB--> [serial_bridge.py] --HTTP POST--> [FastAPI] --> [Supabase/Postgres]
                                                                         ^
                                                                         |
                                                              [index.html] --HTTP GET--
                                                              (muestra el historial)
```

El Arduino **no habla HTTP** (no tiene WiFi en este proyecto), así que un script
Python en la PC hace de "traductor": lee el puerto Serial y reenvía cada nota
a la API por HTTP. La API guarda todo en Supabase y expone un endpoint para
que la web muestre la información persistida.

---

## 1. Base de datos (Supabase)

1. Entrá a tu proyecto en https://supabase.com
2. Andá a **SQL Editor > New query**
3. Pegá y ejecutá el contenido de `database/schema.sql`
4. Andá a **Project Settings > API** y copiá:
   - `Project URL` → esto es tu `SUPABASE_URL`
   - `anon public key` → esto es tu `SUPABASE_KEY`

## 2. Backend (FastAPI)

```bash
cd backend
python -m venv venv
source venv/bin/activate        # En Windows: venv\Scripts\activate

pip install -r requirements.txt

cp .env.example .env
# Editar .env y pegar tu SUPABASE_URL y SUPABASE_KEY reales

uvicorn main:app --reload
```

Esto levanta la API en `http://127.0.0.1:8000`.

Podés probarla directo en el navegador:
- `http://127.0.0.1:8000/notas` → historial (vacío al principio)
- `http://127.0.0.1:8000/docs` → documentación interactiva (Swagger) generada
  automáticamente por FastAPI, útil para probar los endpoints a mano.

## 3. Arduino

1. Abrí `arduino/piano_arduino.ino` en el IDE de Arduino.
2. Subilo a tu placa como siempre (mismo cableado que ya tenías: buzzer en
   pin 13, botones en pines 2, 3, 4 y 5).
3. Lo único que cambió respecto a tu versión original es el formato de los
   mensajes que imprime por Serial: ahora dice `NOTA:DO`, `NOTA:RE`, etc.,
   en vez de solo `DO`, `RE`. Esto es lo que permite que el script de Python
   los reconozca fácilmente.
4. **Importante:** cerrá el Monitor Serie del IDE de Arduino antes de correr
   el script de Python del siguiente paso — el puerto Serial solo lo puede
   usar un programa a la vez.

## 4. Puente Serial → API (serial_bridge.py)

```bash
cd bridge
pip install pyserial requests
```

Antes de correrlo, abrí `serial_bridge.py` y ajustá:

```python
PUERTO_SERIAL = "COM3"   # Windows: COM3, COM4...
                          # Linux/Mac: /dev/ttyUSB0 o /dev/ttyACM0
```

Para saber qué puerto usa tu Arduino: Arduino IDE > Herramientas > Puerto.

Después corrés:

```bash
python serial_bridge.py
```

Vas a ver algo como:

```
Conectando al puerto COM3...
Conectado. Escuchando notas del piano... (Ctrl+C para salir)
[Arduino] NOTA:DO
  -> Guardada en base de datos: DO
```

Cada tecla física que presiones queda guardada en Supabase en este momento.

## 5. Frontend (visualización)

No necesita instalación. Simplemente:

1. Abrí `frontend/index.html` con doble clic (o "Abrir con navegador").
2. La página consulta automáticamente `http://127.0.0.1:8000/notas` cada
   2.5 segundos y muestra:
   - El teclado que se ilumina con la última nota tocada.
   - Un resumen de cuántas veces sonó cada nota.
   - El historial completo con fecha y hora.

Si preferís servirlo en vez de abrirlo como archivo local (recomendado para
evitar problemas de CORS en algunos navegadores):

```bash
cd frontend
python -m http.server 5500
# luego abrir http://127.0.0.1:5500
```

---

## Orden de arranque recomendado

1. Ejecutar el SQL en Supabase (una sola vez).
2. Levantar el backend: `uvicorn main:app --reload`
3. Conectar el Arduino por USB y correr `serial_bridge.py`
4. Abrir `index.html`
5. Tocar el piano físico → ver cómo aparece en vivo en la web.

## Cumplimiento del requisito de la consigna

- **Persistencia de información**: cada nota tocada se guarda en una tabla
  Postgres real dentro de Supabase (`notas_tocadas`), no en memoria.
- **Recuperación para mostrar a usuarios**: el endpoint `GET /notas` y la
  página `index.html` recuperan y muestran esos datos persistidos.
- **Serial o HTTP**: se usan ambos — Serial entre Arduino y la PC, HTTP
  entre la PC (bridge), la API FastAPI y Supabase.

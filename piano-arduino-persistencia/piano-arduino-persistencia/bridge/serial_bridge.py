"""
serial_bridge.py
-----------------
Puente entre el Arduino (puerto Serial) y la API FastAPI.

Qué hace:
1. Se conecta al puerto Serial donde está el Arduino.
2. Lee línea por línea lo que el Arduino imprime (Serial.println).
3. Si la línea tiene el formato "NOTA:XXX", extrae el nombre de la nota.
4. Envía esa nota a la API FastAPI vía HTTP POST para que se guarde en Supabase.

Requisitos:
    pip install pyserial requests

Uso:
    python serial_bridge.py
"""

import serial
import requests
import time

# ----------------------------------------------------------
# CONFIGURACIÓN — AJUSTAR ESTOS VALORES
# ----------------------------------------------------------

# Puerto donde está conectado el Arduino.
# Windows: algo como "COM3", "COM4", etc.
# Linux/Mac: algo como "/dev/ttyUSB0" o "/dev/ttyACM0".
PUERTO_SERIAL = "COM3"

# Debe coincidir con el Serial.begin(9600) del Arduino.
BAUD_RATE = 9600

# URL de tu API FastAPI (por defecto corre local en el puerto 8000).
API_URL = "http://127.0.0.1:8000/notas"

# ----------------------------------------------------------


def main():
    print(f"Conectando al puerto {PUERTO_SERIAL}...")
    arduino = serial.Serial(PUERTO_SERIAL, BAUD_RATE, timeout=1)

    # Esperamos a que el Arduino termine de reiniciarse tras abrir el puerto.
    time.sleep(2)
    print("Conectado. Escuchando notas del piano... (Ctrl+C para salir)")

    while True:
        try:
            linea = arduino.readline().decode("utf-8", errors="ignore").strip()

            if not linea:
                continue

            print(f"[Arduino] {linea}")

            # Solo nos interesan las líneas con formato "NOTA:XXX"
            if linea.startswith("NOTA:"):
                nota = linea.split(":", 1)[1]

                # Ignoramos los silencios para no llenar la base de datos
                # de registros vacíos (esto es una decisión de diseño,
                # se puede cambiar si querés registrar también silencios).
                if nota == "SILENCIO":
                    continue

                enviar_nota_a_api(nota)

        except serial.SerialException as e:
            print(f"Error de conexión Serial: {e}")
            break
        except KeyboardInterrupt:
            print("\nCerrando conexión...")
            break

    arduino.close()


def enviar_nota_a_api(nota: str):
    """Envía la nota tocada a la API FastAPI vía HTTP POST."""
    try:
        respuesta = requests.post(API_URL, json={"nota": nota}, timeout=5)
        if respuesta.status_code == 200 or respuesta.status_code == 201:
            print(f"  -> Guardada en base de datos: {nota}")
        else:
            print(f"  -> Error API ({respuesta.status_code}): {respuesta.text}")
    except requests.exceptions.RequestException as e:
        print(f"  -> No se pudo conectar a la API: {e}")


if __name__ == "__main__":
    main()

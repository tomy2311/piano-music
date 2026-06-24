# 🎹 Piano Electrónico con Arduino

Prototipo de piano electrónico desarrollado con Arduino, capaz de reproducir 
4 notas musicales (Do-Re-Mi-Fa) mediante pulsadores, e incluye una melodía 
de bienvenida al encender el sistema.

## 📋 Descripción

Este proyecto busca ofrecer una herramienta simple y económica para la 
introducción a conceptos musicales básicos, simulando el funcionamiento 
de un teclado mediante un microcontrolador Arduino y un buzzer.

## 🎯 Objetivo

Desarrollar un prototipo funcional que permita generar notas musicales 
(Do-Re-Mi-Fa) a través de pulsadores individuales, junto con una secuencia 
de bienvenida programada, utilizando componentes electrónicos básicos.

## 🛠️ Componentes utilizados

- Arduino Uno
- Buzzer (2 pines)
- 4 Pulsadores (push buttons)
- Mini protoboard
- Cables macho-hembra / hembra-hembra
- Resistencias (si corresponde a los pulsadores)

## ⚙️ Funcionamiento

1. Al encender el circuito, el buzzer reproduce una melodía de bienvenida.
2. Cada uno de los 4 pulsadores está asociado a una nota musical (Do, Re, Mi, Fa).
3. Al presionar un pulsador, el buzzer emite la frecuencia correspondiente 
   a esa nota mediante la función `tone()` de Arduino.

## 📂 Material adicional

En este repositorio vas a encontrar un archivo comprimido (`.zip` o `.rar`) 
con videos e imágenes del proyecto en funcionamiento, incluyendo:

- Fotos del circuito armado
- Video de demostración del piano funcionando

📥 **Para visualizarlos:** descargá el archivo comprimido del repositorio 
y extraelo en tu computadora con un programa como WinRAR, 7-Zip o el 
descompresor nativo de tu sistema operativo.

## 📄 Documentación

Este repositorio incluye también la documentación completa del proyecto 
en formato PDF/Word, donde se detalla:

- Justificación del problema y la solución
- Objetivos del proyecto
- Componentes utilizados y su función
- Diagrama de conexiones
- Código comentado
- Proceso de Armado
- Diagrama del Flujo de codigo

📥 **Para visualizarla:** descargá el archivo de documentación 
ubicado en la carpeta `documentacion/` del repositorio.

## 🚀 Cómo usarlo

1. Cloná este repositorio.
2. Abrí el archivo `.ino` en el IDE de Arduino.
3. Conectá tu Arduino Uno por USB.
4. Subí el código a la placa.
5. ¡Listo! Presioná los pulsadores para tocar las notas.

## 📌 Estado del proyecto

🚧 En desarrollo

## 👤 Autores

Garcia Thomas, Racana Esteban

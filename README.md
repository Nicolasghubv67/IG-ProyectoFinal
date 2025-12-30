# IG-ProyetoFinal

## Commit 1

### Normal Mapping (TBN)
- `vshader.glsl`: añade atributos `inTangent`(loc=3) e `inBitangent`(loc=4), construye base TBN en mundo, la pasa como `vTBN`.
- `fshader.glsl`: convierte el normal map de [0..1] a [-1..1]. Con TBN se transforma del espacio tangente en el que vive el normal map al espacio mundo. Con la nueva normal se aplica mejor iluminación.

### Modelos (VAO/VBO)
- `Model.{h,cpp}`: guarda `tangents/bitangents`, crea VBOs extra, activa atributos 3 y 4.
- Usa tangentes/bitangentes de Assimp si existen; fallback si no.

### Texturas
- `Shaders::setTextures`: deja de usar el texture ID como unidad de textura.
- Fija unidades: diffuse=0, specular=1, emissive=2, normal=3 (y actualiza los `sampler2D`).

### Escena y controles
- `main.cpp`: añade luces (direccional, posicional con movimiento y focal), carga texturas (suelo y pared ladrillo), material `gold`.
- Cámara orbital por ratón (botón izq).
- `N` activa/desactiva normal mapping global (`uWithNormals`).
- Flechas mueven la luz posicional en X/Z y se actualiza su esfera.

## Commit 2

# Cambios en `main.cpp` – Resumen

Este commit introduce una **reestructuración completa del sistema de cámara e interacción**, manteniendo intactos el renderizado, los shaders, la iluminación y el modelado de la escena.


## 1. Cambio de cámara: orbital → FPS

- Se elimina la cámara orbital basada en ángulos `alphaX / alphaY`.
- Se implementa una **cámara tipo FPS**, definida por:
    - Posición (`camPos`)
    - Dirección de visión (`camFront`)
    - Vector vertical (`camUp`)
    - Ángulos `yaw` y `pitch`
- La matriz de vista pasa a construirse con `lookAt(camPos, camPos + camFront, camUp)`.

Resultado: la cámara ya no gira alrededor del origen, sino que se mueve libremente por la escena.

---

## 2. Movimiento independiente del framerate (deltaTime)

- Se añade cálculo de tiempo por frame (`deltaTime`).
- Todo el movimiento de cámara y ajustes depende de `deltaTime`.
- Se limita el valor máximo para evitar saltos tras parones.

Resultado: movimiento fluido y consistente independientemente de los FPS.

---

## 3. Separación entre eventos y lógica por frame

Se introducen dos funciones nuevas:
- `processCameraMove`: movimiento continuo de la cámara (WASD, espacio, shift).
- `processTuning`: ajuste progresivo de sensibilidad del ratón y velocidad de cámara.

Resultado: mejora la estructura.

---

## 4. Sistema de control del ratón

- El ratón se **captura con clic izquierdo** y se libera con `ESC`.
- Se añade control del primer frame (`firstMouse`) para evitar saltos bruscos al capturar.
- La rotación se basa en desplazamientos relativos del cursor.

Resultado: rotación suave y estable de la cámara en modo FPS.

---

## 5. Proyección dinámica (FOV)

- El campo de visión (`fovy`) pasa a ser dinámico.
- Se controla con la rueda del ratón.
- Se limita entre 30° y 90°.
- El título de la ventana muestra FOV, sensibilidad y velocidad actuales.

---

## 6. Ajustes adicionales

- `renderScene` recibe ahora el puntero a la ventana (`GLFWwindow*`).
- Se amplía el plano lejano de la proyección (`far = 100`).
- Se añade callback para botones del ratón.

---

## 7. Elementos no modificados

- No se cambian:
    - Shaders
    - Iluminación
    - Texturas
    - Modelos
    - Transformaciones de los objetos (`M = T · R · S`)

El cambio afecta **únicamente a la cámara y a la interacción**.

---
## Commit 3


## Refactorización de cámara e interacción

Refactorización para separar completamente la **cámara e interacción con el usuario** del archivo `main.cpp`.

### Cambios realizados
- Toda la lógica de cámara FPS (posición, orientación, yaw/pitch).
- Movimiento con teclado (WASD, espacio, shift).
- Captura y liberación del ratón.
- Control de FOV con la rueda.
- Ajuste dinámico de sensibilidad y velocidad con aceleración progresiva `+'¡.

ha sido extraída a una clase independiente `CameraFPS`.

### Nueva clase `CameraFPS`
La clase encapsula:
- Estado completo de la cámara.
- Callbacks de ratón y scroll.
- Actualización por frame basada en `deltaTime`.
- Generación de la matriz de vista (`getView()`).

Se implementa como **header-only**.



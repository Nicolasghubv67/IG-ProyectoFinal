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

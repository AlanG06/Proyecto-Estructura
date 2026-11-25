# Tabla de pruebas

Se cargan los 8 paquetes de prueba al iniciar (respuesta `s`). Las pruebas se hacen en este orden.

| # | Operación | Datos ingresados | Resultado esperado | Resultado obtenido |
|---|---|---|---|---|
| 1 | Registrar folio repetido (op. 1) | Folio 50 | Se rechaza el registro duplicado | "Ese folio ya existe" ✔ |
| 2 | Registrar con peso inválido (op. 1) | Folio 90, "Caja", peso 0 | Se rechaza, el peso debe ser mayor a 0 | "El peso debe ser mayor a 0" ✔ |
| 3 | Registrar con origen = destino (op. 1) | Folio 91, "Caja", 2 kg, origen 3, destino 3 | Se rechaza | "Origen y destino deben ser distintos" ✔ |
| 4 | Buscar folio existente (op. 2) | 45 | Muestra todos los datos del paquete | Documentos, 0.80 kg, Puerto → Centro, Registrado ✔ |
| 5 | Buscar folio inexistente (op. 2) | 99 | Avisa que no existe | "No existe ese folio" ✔ |
| 6 | Mostrar recorridos (op. 3) | — | Pre: 50 30 20 40 45 70 60 80 / In: 20 30 40 45 50 60 70 80 / Post: 20 45 40 30 60 80 70 50 | Iguales al árbol dibujado ✔ |
| 7 | Matriz y grados (op. 11) | — | Matriz inicial; Poniente entrada 0, Puerto salida 0 | Coincide con la matriz del diseño ✔ |
| 8 | Encolar tres paquetes (op. 4 y 5) | 50, 70, 30 | Cola en orden de llegada, estado En espera | Frente → 50 70 30 ← Final ✔ |
| 9 | Despachar paquete con escala (op. 6) | Frente = 50 (Centro → Puerto) | Imprime la ruta completa y pasa al historial | Centro → Norte → Puerto, "Paquete despachado" ✔ |
| 10 | Despachar paquete sin ruta (op. 6 y 5) | Frente = 70 (Centro → Poniente) | Pasa al final de la cola y sigue En espera | "no hay ruta, pasa al final"; cola: 30 70 ✔ |
| 11 | Agregar conexión (op. 9) | Origen 3 (Oriente), destino 4 (Poniente) | Se agrega la conexión | "Conexion agregada: Oriente -> Poniente" ✔ |
| 12 | Despachar los que siguen (op. 6 dos veces) | Frente = 30, luego 70 | 30 sale directo; 70 ya tiene ruta | Centro → Norte; Centro → Sur → Oriente → Poniente ✔ |
| 13 | Historial después de los despachos (op. 8) | — | El último despacho aparece primero | 70 (cima), 30, 50 ✔ |
| 14 | Último despacho (op. 7) | — | Datos del folio de la cima | Folio 70, Refacciones, Despachado ✔ |
| 15 | Encolar paquete ya despachado (op. 4) | 50 | Se rechaza, solo se encolan Registrados | "Solo se encolan paquetes Registrados (este esta Despachado)" ✔ |
| 16 | Eliminar conexión que no existe (op. 10) | Origen 5, destino 0 | Avisa que no existe | "Esa conexion no existe" ✔ |
| 17 | Grados después de agregar conexión (op. 11) | — | Oriente salida 2, Poniente entrada 1 | Oriente 1/2, Poniente 1/1 ✔ |
| 18 | Reporte por peso con empate (op. 12) | — | 45, 20, 40, 50, 60, 30, 70, 80 (40 antes que 50) | Mismo orden, el empate se resuelve por folio ✔ |
| 19 | Opción con letras | `abc` | Pide el número otra vez | "Numero invalido, otra vez" ✔ |
| 20 | Opción fuera de rango | 20 | Avisa que no es válida | "Opcion no valida" ✔ |
| 21 | Despachar con cola vacía (op. 6) | — | Avisa que la cola está vacía | "La cola esta vacia" ✔ |
| 22 | Encolar con la cola llena (op. 4) | 11 paquetes Registrados, capacidad 10 | El 11° no entra y sigue Registrado | "La cola esta llena, sigue como Registrado" ✔ |

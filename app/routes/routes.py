from flask import Blueprint, request, jsonify, render_template
from app.services.esp32 import ConexionESP32
from app.config import PUERTO_ESP32, VELOCIDAD_BAUDIOS

app_blueprint = Blueprint('app', __name__)

# Instancia global de ESP32 (se inicializará al conectarse)
esp32 = None

@app_blueprint.route('/')
def index():
    return render_template('index.html')

@app_blueprint.route("/conectar", methods=["GET"])
def conectar():
    global esp32
    if esp32:
        return jsonify({"estado": "Conexión ya iniciada"})
    try:
        esp32 = ConexionESP32(PUERTO_ESP32, VELOCIDAD_BAUDIOS)
        if esp32.iniciar_conexion():
            return jsonify({"estado": "Conexión iniciada"})
        else:
            return jsonify({"estado": "Error al iniciar la conexión"}), 500
    except Exception as e:
        return jsonify({"estado": f"Error al iniciar la conexión: {e}"}), 500

@app_blueprint.route("/desconectar", methods=["GET"])
def desconectar():
    global esp32
    if esp32 and esp32.cerrar_conexion():
        esp32 = None
        return jsonify({"estado": "Conexión cerrada"})
    return jsonify({"estado": "No hay conexión activa"}), 400

@app_blueprint.route("/play/<pista>", methods=["POST"])
def play(pista):
    if not esp32:
        return jsonify({"error": "No hay conexión activa"}), 400
    respuesta = esp32.play(pista)
    return jsonify(respuesta)

@app_blueprint.route("/pause", methods=["POST"])
def pause():
    if not esp32:
        return jsonify({"error": "No hay conexión activa"}), 400
    respuesta = esp32.pause()
    return jsonify(respuesta)

@app_blueprint.route("/resume", methods=["POST"])
def resume():
    if not esp32:
        return jsonify({"error": "No hay conexión activa"}), 400
    respuesta = esp32.resume()
    return jsonify(respuesta)

@app_blueprint.route("/volumen", methods=["POST"])
def volumen():
    if not esp32:
        return jsonify({"error": "No hay conexión activa"}), 400
    try:
        data = request.get_json()
        nivel = data.get("nivel")
        if nivel is None:
            return jsonify({"error": "Nivel de volumen no proporcionado"}), 400
        respuesta = esp32.volumen(nivel)
        return jsonify(respuesta)
    except Exception as e:
        return jsonify({"error": f"Error al ajustar el volumen: {e}"}), 500

@app_blueprint.route("/status", methods=["GET"])
def status():
    if not esp32:
        return jsonify({"error": "No hay conexión activa"}), 400
    respuesta = esp32.status()
    return jsonify(respuesta)

@app_blueprint.route("/reset", methods=["POST"])
def reset():
    if not esp32:
        return jsonify({"error": "No hay conexión activa"}), 400
    respuesta = esp32.reset()
    return jsonify(respuesta)

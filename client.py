import socket

# Configuración de la conexión
esp32_ip = "192.168.1.123"  # Cambia a la dirección IP de tu ESP32
esp32_port = 5000           # Cambia al puerto que usas en tu ESP32

# Crear socket y conectar
try:
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((esp32_ip, esp32_port))
    print(f"Conectado a {esp32_ip}:{esp32_port}")

    # Enviar comandos
    while True:
        command = input("Escribe un comando (play, pause, stop, exit): ").strip()
        if command.lower() == "exit":
            print("Cerrando conexión.")
            break
        client_socket.sendall(command.encode('utf-8'))  # Envía el comando
        print(f"Comando enviado: {command}")

except Exception as e:
    print(f"Error: {e}")
finally:
    client_socket.close()

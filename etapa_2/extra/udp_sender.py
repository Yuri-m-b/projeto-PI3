"""Script para envio de dados em formato UDP."""

import socket

SERVER_IP = "192.168.0.241"  # Endereço IP do Raspberry com VxWorks
SERVER_PORT = 21  # Port utilizada pelo VxWorks
MESSAGE = "Hello, VxWorks!"  # Mensagem/dado que será enviado
REPEAT = 10


# Envia mensagem para o receptor
def main():
    """Main function"""
    # Cria um socket UDP
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    count = 0
    while count < REPEAT:
        sock.sendto(MESSAGE.encode(), (SERVER_IP, SERVER_PORT))
        print(f"Mensagem Enviada: {MESSAGE}")
        count += 1

    sock.close()

    return 0


if __name__ == "__main__":
    main()

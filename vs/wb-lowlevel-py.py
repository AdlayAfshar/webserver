import socket

PORT = 8000

# 1. socket
server_fd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# 2. bind
server_fd.bind(("0.0.0.0", PORT))

# 3. listen
server_fd.listen(10)

print("Server running on http://localhost:8000")

while True:
    # 4. accept
    client_fd, addr = server_fd.accept()

    # 5. read request
    buffer = client_fd.recv(30000)

    # فقط خط اول رو چک می‌کنیم
    if buffer.startswith(b"GET /products"):
        response = (
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "\r\n"
            "[{\"id\":1,\"name\":\"Shoes\"},{\"id\":2,\"name\":\"Bag\"}]"
        )
    else:
        response = (
            "HTTP/1.1 404 Not Found\r\n"
            "\r\n"
        )

    # write
    client_fd.sendall(response.encode())

    # close
    client_fd.close()
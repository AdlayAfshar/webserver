from http.server import BaseHTTPRequestHandler, HTTPServer
import json

class MyHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == "/products":
            # داده
            data = [
                {"id": 1, "name": "Shoes"},
                {"id": 2, "name": "Bag"}
            ]

            # تبدیل به JSON
            json_data = json.dumps(data)

            # status
            self.send_response(200)

            # header
            self.send_header("Content-Type", "application/json")
            self.end_headers()

            # body
            self.wfile.write(json_data.encode())

        else:
            self.send_response(404)
            self.end_headers()

server = HTTPServer(("localhost", 8000), MyHandler)
print("Server running on http://localhost:8000")
server.serve_forever()
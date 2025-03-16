from http.server import BaseHTTPRequestHandler, HTTPServer

class SimpleHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        content_length = int(self.headers['Content-Length'])  # Get data length
        post_data = self.rfile.read(content_length)  # Read data
        print(f"Received: {post_data.decode()}")  # Log data to terminal
        
        self.send_response(200)
        self.end_headers()
        self.wfile.write(b"Data received")  # Send response

server = HTTPServer(("0.0.0.0", 8080), SimpleHandler)
print("Server running on port 8080...")
server.serve_forever()

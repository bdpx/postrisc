#!/usr/bin/python3

import http.server
import socketserver
import os
import sys

def main():
    # Define the port the server will listen on
    PORT = 8000
    root_folder="artifacts"

    # Define the path to the directory you want to serve
    # This example uses a subdirectory named 'web_content' relative to the script's location.
    # You can change this to any absolute or relative path.
    script_dir = os.path.dirname(__file__)  # Get the directory where the script is located
    web_dir = os.path.join(script_dir, root_folder) 

    # Change the current working directory to the desired web directory
    os.chdir(web_dir)

    # Create the HTTP request handler
    MyHandler = http.server.SimpleHTTPRequestHandler

    # Create the TCP server instance
    with socketserver.TCPServer(("", PORT), MyHandler) as httpd:
        print(f"Serving directory: {web_dir}, port: {PORT}")
        # Start the server and keep it running indefinitely
        httpd.allow_reuse_address = True
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\nCtrl-C received, shutting down server...")
        finally:
            httpd.shutdown()
            httpd.server_close()
            print("Server successfully shut down.")
            sys.exit(0)


if __name__ == "__main__":
    main()
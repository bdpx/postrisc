#!/usr/bin/python3

import http.server
import socketserver
import os

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
Handler = http.server.SimpleHTTPRequestHandler

# Create the TCP server instance
with socketserver.TCPServer(("", PORT), Handler) as httpd:
    print(f"Serving directory: {web_dir}, port: {PORT}")
    # Start the server and keep it running indefinitely
    httpd.serve_forever()
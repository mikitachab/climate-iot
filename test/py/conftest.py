import os
import socket
import subprocess

import pytest


@pytest.fixture
def port():
    return get_free_port()


@pytest.fixture()
def climate_server(port):
    climate_server_bin = os.environ["CLIMATE_API_BIN"]
    climate_server_cmd = f"{climate_server_bin} --port {port}"
    p = subprocess.Popen(climate_server_cmd, shell=True)

    yield

    p.kill()


@pytest.fixture
def live_url(climate_server, port):
    return f"http://localhost:{port}"


def get_free_port():
    s = socket.socket()
    s.bind(("", 0))
    port = s.getsockname()[1]
    s.close()
    return port

import requests


def test_ping_pong(live_url):
    resp = requests.get(live_url + "/ping")
    assert resp.content == b"PONG"

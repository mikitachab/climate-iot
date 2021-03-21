import requests
import random


def test_add_device(live_url):
    resp = requests.post(live_url + "/devices", json={"name": "dev-0"})

    assert resp.status_code == 201
    assert resp.json().get("name") == "dev-0"
    assert "id" in resp.json().keys()


def test_can_get_added_devices(live_url):
    dev_1, dev_2 = str(random.randint(1, 100)), str(random.randint(1, 100))
    requests.post(live_url + "/devices", json={"name": dev_1})
    requests.post(live_url + "/devices", json={"name": dev_2})

    resp = requests.get(live_url + "/devices")

    assert resp.status_code == 200

    devices = resp.json()
    assert len(devices) == 2
    assert sorted([dev_1, dev_2]) == sorted([d["name"] for d in devices])


def test_unique_device(live_url):
    requests.post(live_url + "/devices", json={"name": "dev-0"})
    
    resp = requests.post(live_url + "/devices", json={"name": "dev-0"})

    assert resp.status_code == 400
    assert resp.json().get("message") is not None

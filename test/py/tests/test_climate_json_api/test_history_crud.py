import requests


def test_add_history_record(live_url):
    requests.post(live_url + "/devices", json={"name": "dev-0"})

    resp = requests.post(live_url + f"/devices/dev-0/history", json={"temperature": 30, "datetime": "2016-08-30 18:47:56"})
    assert resp.status_code == 201


def test_get_added_history_records(live_url):
    requests.post(live_url + "/devices", json={"name": "dev-0"})

    records = [
        {"temperature": 30, "datetime": "2016-08-30 18:47:56"},
        {"temperature": 30, "datetime": "2016-08-30 18:47:57"}
    ]

    for record in records:
        requests.post(live_url + f"/devices/dev-0/history", json=record)

    resp = requests.get(live_url + f"/devices/dev-0/history")
    assert resp.status_code == 200
    assert sorted(resp.json(), key=lambda x: x["datetime"]) == sorted(records, key=lambda x: x["datetime"])

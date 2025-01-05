import requests
import json
import time

BASE_URL = "http://127.0.0.1:5000"  # Replace with your Flask server's URL if deployed elsewhere

def test_sign_api():
    print("Testing /sign endpoint...\n")

    # Prepare the test message
    message = "Hello, this is a test message!"

    # Send a POST request to the /sign endpoint
    response = requests.post(
        f"{BASE_URL}/sign",
        json={"message": message},
        headers={"Content-Type": "application/json"}
    )

    # Check the response
    if response.status_code == 200:
        data = response.json()
        print("Message:", data.get("message"))
        print("Signature:", data.get("signature"))
        print("Public Key e:", data.get("e"))
        print("Public Key n:", data.get("n"))
        print("\n/sign endpoint test passed!\n")
        return data
    else:
        print("Error:", response.status_code, response.text)
        raise Exception("/sign endpoint test failed")


def test_verify_api(message, signature, public_key_e, public_key_n):
    print("Testing /verify endpoint...\n")

    # Send a POST request to the /verify endpoint
    response = requests.post(
        f"{BASE_URL}/verify",
        json={
            "message": message,
            "signature": signature,
            "e": public_key_e,
            "n": public_key_n
        },
        headers={"Content-Type": "application/json"}
    )

    # Check the response
    if response.status_code == 200:
        data = response.json()
        print("Verification Result:", data.get("valid"))
        print("Message:", data.get("message"))
        if data.get("valid"):
            print("\n/verify endpoint test passed!\n")
        else:
            print("\n/verify endpoint test failed (Invalid signature)!\n")
    else:
        print("Error:", response.status_code, response.text)
        raise Exception("/verify endpoint test failed")


if __name__ == "__main__":
    # Test the /sign endpoint
    start_time = time.time()
    signed_data = test_sign_api()

    # Extract the signed data
    message = signed_data["message"]
    signature = signed_data["signature"]
    public_key_e = signed_data["e"]
    public_key_n = signed_data["n"]

    # Test the /verify endpoint with the signed data
    test_verify_api(message, signature, public_key_e, public_key_n)

    # Kết thúc tính thời gian
    end_time = time.time()

    # Tính thời gian chạy
    elapsed_time = end_time - start_time
    print(f"Elapsed time: {elapsed_time:.6f} seconds")

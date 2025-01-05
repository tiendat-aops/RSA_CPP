from flask import Flask, request, jsonify
from Crypto.Hash import SHA256
import base64
import generate_large_prime
from rsa_uet import BigNumber

app = Flask(__name__)

# Tạo khóa RSA 
p = BigNumber(str(generate_large_prime.generate(4096)))
# print(p)
q = BigNumber(str(generate_large_prime.generate(4096)))
# print(q)
public_key = BigNumber(str(generate_large_prime.generate(8192)))

n, phi_n, private_key = public_key.RSA(p, q)

x = BigNumber("123")
y = x.modularExponentiation(public_key, n)
x_ = y.modularExponentiation(private_key, n)
print(x_.getValue() == x.getValue())

@app.route('/sign', methods=['POST'])
def sign_message():
    """API để ký một message"""
    # try:
    # Lấy message từ body request
    data = request.json
    message = data.get("message")
    
    if not message:
        return jsonify({"error": "Message is required"}), 400
    
    # Tạo hash của message
    message_hash = SHA256.new(message.encode('utf-8'))
    message_dec = int(message_hash.hexdigest(), 16)
    print("Message in decimal in sign: ", message_dec)

    # Ký hash bằng khóa riêng
    message_ = BigNumber(str(message_dec))
    signature = message_.modularExponentiation(private_key, n)
    
    signature_str = str(signature.getValue())
    
    return jsonify({
        "message": message,
        "signature": signature_str,
        "e": public_key.getValue(),
        "n": n.getValue()
    })
    # except Exception as e:
    #     return jsonify({"error": str(e)}), 500


@app.route('/verify', methods=['POST'])
def verify_signature():
#     """API để kiểm tra chữ ký của một message"""
#     try:
    # Lấy thông tin từ body request
    data = request.json
    message = data.get("message")
    signature = data.get("signature")
    public_key_e = data.get("e")
    public_key_n = data.get("n")
    
    if not message or not signature or not public_key_e or not public_key_n:
        return jsonify({"error": "Message, signature, and public_key are required"}), 400
    
    # Tạo hash của message
    message_hash = SHA256.new(message.encode('utf-8'))
    message_dec = int(message_hash.hexdigest(), 16)

    # Ký hash bằng khóa riêng
    # message_ = BigNumber(str(message_dec))
    
    # Xác minh chữ ký
    try:
        signature_ = BigNumber(str(signature))
        decrypt_message = signature_.modularExponentiation(BigNumber(str(public_key_e)), BigNumber(str(public_key_n)))
        print("Decryp message: ", decrypt_message)
        print("Message decimal in verify: ", message_dec)
        if decrypt_message.getValue() == str(message_dec):
            return jsonify({"valid": True, "message": "Signature is valid"})
        else:
            return jsonify({"valid": False, "message": "Signature is invalid"})
    except (ValueError, TypeError):
        return jsonify({"valid": False, "message": "Signature is invalid"})
#     except Exception as e:
#         return jsonify({"error": str(e)}), 500


if __name__ == '__main__':
    app.run(debug=True)

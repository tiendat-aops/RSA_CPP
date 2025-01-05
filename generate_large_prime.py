from sympy import randprime

def generate(bits):
    # Generate a random prime number of specified bit length
    lower_bound = 2**(bits - 1)
    upper_bound = 2**bits - 1
    return randprime(lower_bound, upper_bound)

# print(generate_large_prime(1024))
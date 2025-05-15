# from django.http import JsonResponse
# import requests

# def get_jwt_token(request):
#     url = "http://127.0.0.1:8000/api/token/"
#     data = {
#         "username": "your_username",
#         "password": "your_password"
#     }
#     response = requests.post(url, json=data)
    
#     if response.status_code == 200:
#         return JsonResponse(response.json())  # Return tokens to frontend
#     else:
#         return JsonResponse({"error": "Invalid credentials"}, status=400)

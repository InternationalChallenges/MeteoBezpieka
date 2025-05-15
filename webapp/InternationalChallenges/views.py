from rest_framework.decorators import api_view, permission_classes
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response
from .models import SensorData
from .serializers import SensorDataSerializer
from rest_framework_simplejwt.views import TokenObtainPairView, TokenRefreshView
from django.urls import path
from django.http import JsonResponse, HttpResponse
import requests
from django.contrib.auth.views import PasswordChangeView
from django.urls import reverse_lazy
from django.views.generic import CreateView
from django.contrib.auth.forms import UserCreationForm
from django.contrib.auth.models import User
from .models import SensorData
from django.shortcuts import render

class SignUpView(CreateView):
    model = User
    form_class = UserCreationForm  # No need for a custom form
    template_name = "registration/signup.html"
    success_url = reverse_lazy("home")  # Redirects to home after signup


# def get_jwt_token(request):
#     url = "http://127.0.0.1:8000/api/token/"
#     data = {
#         "username": "user",
#         "password": "password"
#     }
#     response = requests.post(url, json=data)
    
#     if response.status_code == 200:
#         return JsonResponse(response.json())  # Return tokens to frontend
#     else:
#         return JsonResponse({"error": "Invalid credentials"}, status=400)



def home(request):
    return HttpResponse("Welcome to InternationalChallenges API!")

@api_view(['GET'])
@permission_classes([IsAuthenticated])
def protected_view(request):
    return Response({"message": "You have access to this API!"})


@api_view(['POST'])
def receive_sensor_data(request):
    serializer = SensorDataSerializer(data=request.data)
    if serializer.is_valid():
        serializer.save()
        return Response({"message": "Data saved successfully"}, status=201)
    return Response(serializer.errors, status=400)


urlpatterns = [
    path('api/token/', TokenObtainPairView.as_view(), name='token_obtain_pair'),  # Login to get access & refresh token
    path('api/token/refresh/', TokenRefreshView.as_view(), name='token_refresh'),  # Refresh token
]


def get_sensor_data(request):
    # Récupérer les 10 dernières entrées dans la base de données
    sensor_data = SensorData.objects.all().order_by('-timestamp')[:10]
            
    # Créer une liste avec les données à renvoyer
    data = list(sensor_data.values('temperature', 'humidity', 'pressure', 'RSSI', 'SNR', 'timestamp'))
    
    # Reverse data order
    data = data[::-1]
    
    # Renvoyer les données sous forme de réponse JSON
    return JsonResponse(data, safe=False)

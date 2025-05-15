import jwt
from django.conf import settings
from django.contrib.auth import get_user_model
from django.utils.deprecation import MiddlewareMixin
from rest_framework_simplejwt.authentication import JWTAuthentication
from rest_framework_simplejwt.exceptions import InvalidToken, AuthenticationFailed

class JWTAuthMiddleware(MiddlewareMixin):
    def process_request(self, request):
        header = request.META.get("HTTP_AUTHORIZATION", "")
        if header.startswith("Bearer "):
            token = header.split(" ")[1]
            try:
                user_auth_tuple = JWTAuthentication().authenticate(request)
                if user_auth_tuple is not None:
                    request.user, _ = user_auth_tuple
                    print("JWTAuthMiddleware: user authenticated:", request.user)

            except (InvalidToken, AuthenticationFailed) as e:
                print("JWTAuthMiddleware: invalid token", str(e))
                pass

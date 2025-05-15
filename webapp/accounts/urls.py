# accounts/urls.py
from django.urls import path

from .views import (
    SignUpView,
    PasswordChangeView,
    CustomPasswordChangeView,
    PasswordChangeDoneView,
    AccountDeleteView,
    AccountDeleteDoneView,
)

urlpatterns = [
    path("signup/", SignUpView.as_view(), name="signup"),
    path("password_change/", CustomPasswordChangeView.as_view(), name="password_change"),
    path(
        "password_change/done/",
        PasswordChangeDoneView.as_view(),
        name="password_change_done",
    ),

    path("delete/", AccountDeleteView.as_view(), name="account_delete"),
    path("delete/done/", AccountDeleteDoneView.as_view(), name="account_delete_done"),

]


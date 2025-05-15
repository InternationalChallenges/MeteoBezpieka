# accounts/views.py
from django.contrib.auth.forms import UserCreationForm
from django.urls import reverse_lazy
from django.views.generic import CreateView
from django.contrib.auth.views import (
    PasswordChangeView as BasePasswordChangeView,
    PasswordChangeDoneView as BasePasswordChangeDoneView,
)

# These views are heavily based on Django's defaults (the forms come from Django).
# If we'd like to change it or style it better, we should probably create custom forms
# which extend the original ones and then use them in these views. I haven't done it before,
# I guess it needs a few good google searches if you want to get it right.

class SignUpView(CreateView):
    form_class = UserCreationForm
    success_url = reverse_lazy("login")
    template_name = "registration/signup.html"


class PasswordChangeView(BasePasswordChangeView):
    success_url = reverse_lazy("password_change_done")
    template_name = "registration/password_change_form.html"


class PasswordChangeDoneView(BasePasswordChangeDoneView):
    template_name = "registration/password_change_done.html"

from django.contrib.auth.mixins import LoginRequiredMixin
from django.views.generic import TemplateView
from django.views.generic.edit import FormView
from django import forms
from django.contrib.auth import logout
from django.contrib.auth.views import PasswordChangeView
from django.urls import reverse_lazy
from .forms import CustomPasswordChangeForm  # Import your custom form

class CustomPasswordChangeView(PasswordChangeView):
    form_class = CustomPasswordChangeForm
    success_url = reverse_lazy('registration/password_change_done.html')
    template_name = 'registration/password_change_form.html'  # Corrected


class AccountDeleteForm(forms.Form):
    confirm = forms.BooleanField(label="I confirm that I want to delete my account")

class AccountDeleteView(LoginRequiredMixin, FormView):
    template_name = "registration/account_delete.html"
    form_class = AccountDeleteForm
    success_url = reverse_lazy("account_delete_done")

    def form_valid(self, form):
        user = self.request.user
        logout(self.request)  # Log out the user before deleting their account
        user.delete()
        return super().form_valid(form)

class AccountDeleteDoneView(TemplateView):
    template_name = "registration/account_delete_done.html"

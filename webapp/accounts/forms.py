from django import forms
from django.contrib.auth.forms import PasswordChangeForm

class CustomPasswordChangeForm(PasswordChangeForm):
    old_password = forms.CharField(
        label="Enter Old Password",  # Change label here
        widget=forms.PasswordInput(attrs={'class': 'form-control'})
    )
    new_password1 = forms.CharField(
        label="Enter New Password",  # Change label here
        widget=forms.PasswordInput(attrs={'class': 'form-control'})
    )
    new_password2 = forms.CharField(
        label="Confirm New Password",  # Change label here
        widget=forms.PasswordInput(attrs={'class': 'form-control'})
    )
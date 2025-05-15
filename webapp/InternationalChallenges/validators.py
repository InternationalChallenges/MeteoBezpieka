import re
from django.core.exceptions import ValidationError
from django.utils.translation import gettext as _

class ComplexityValidator:
    def validate(self, password, user=None):
        if not re.search(r'[A-Z]', password):
            raise ValidationError(_('Le mot de passe doit contenir au moins une lettre majuscule.'), code='password_no_upper')
        if not re.search(r'[a-z]', password):
            raise ValidationError(_('Le mot de passe doit contenir au moins une lettre minuscule.'), code='password_no_lower')
        if not re.search(r'\d', password):
            raise ValidationError(_('Le mot de passe doit contenir au moins un chiffre.'), code='password_no_digit')
        if not re.search(r'[^\w\s]', password):
            raise ValidationError(_('Le mot de passe doit contenir au moins un caractère spécial (ex: @, #, !).'), code='password_no_special')

    def get_help_text(self):
        return _(
            "Le mot de passe doit contenir au moins une lettre majuscule, une lettre minuscule, un chiffre et un caractère spécial."
        )


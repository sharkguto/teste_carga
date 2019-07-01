from django.shortcuts import render
from django.http import JsonResponse


def get_hello(request, name: str) -> tuple:  # noqa: E501
    """

     # noqa: E501

    :param name: 
    :type name: str

    :rtype: None
    """
    return JsonResponse({"response": f" ola {name}. bem vindo ao clube"})

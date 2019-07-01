from django.shortcuts import render, HttpResponse

# Create your views here.


def calc(request):

    return HttpResponse("request", status=200)


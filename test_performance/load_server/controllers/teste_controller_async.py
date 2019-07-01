def get_hello(name: str) -> tuple:  # noqa: E501
    """

     # noqa: E501

    :param name: 
    :type name: str

    :rtype: None
    """
    return {"response": f" ola {name}. bem vindo ao clube"}, 200


# import aiohttp
# from connexion.lifecycle import ConnexionResponse


# async def get_hello(name: str) -> tuple:  # noqa: E501
#     """

#      # noqa: E501

#     :param name: 
#     :type name: str

#     :rtype: None
#     """
#     return aiohttp.web.json_response(
#         {"response": f" ola {name}. bem vindo ao clube"}, 200
#     )


# async def get_hello(name: str) -> tuple:  # noqa: E501
#     """

#      # noqa: E501

#     :param name: 
#     :type name: str

#     :rtype: None
#     """
#     return aiohttp.web.json_response(
#         {"response": f" ola {name}. bem vindo ao clube"}, 200
#     )
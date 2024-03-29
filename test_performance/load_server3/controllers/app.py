from connexion.decorators.security import validate_scope
from connexion.exceptions import OAuthScopeProblem

# from aiohttp import web
from connexion.lifecycle import ConnexionResponse


def basic_auth(username, password, required_scopes=None):
    if username == "admin" and password == "secret":
        info = {"sub": "admin", "scope": "secret"}
    elif username == "foo" and password == "bar":
        info = {"sub": "user1", "scope": ""}
    else:
        # optional: raise exception for custom error response
        return None

    # optional
    if required_scopes is not None and not validate_scope(
        required_scopes, info["scope"]
    ):
        raise OAuthScopeProblem(
            description="Provided user doesn't have the required access rights",
            required_scopes=required_scopes,
            token_scopes=info["scope"],
        )

    return info


def dummy_func(token):
    return None


# def get_secret(user) -> str:
#     return "You are {user} and the secret is 'wbevuec'".format(user=user)


async def get_secret(user) -> str:
    # return "You are {user} and the secret is 'wbevuec'".format(user=user)
    return ConnexionResponse(
        body="You are {user} and the secret is 'wbevuec'".format(user=user)
    )


# gunicorn load_server3:application --workers=4 -b "0.0.0.0:8080" --worker-class=aiohttp.GunicornUVLoopWebWorker --chdir load_server3/
# gunicorn load_server3:application --workers=4 -b "0.0.0.0:8080" --worker-class=aiohttp.worker.GunicornWebWorker --chdir load_server3/


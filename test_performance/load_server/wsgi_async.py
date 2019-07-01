
import connexion
import sys
import os

sys.path.insert(0, f"{os.getcwd()}")


app = connexion.App(__name__, specification_dir="./openapi/")
app.add_api("openapi.yaml", arguments={"title": "teste de carga"})
application = app

if __name__ == "__main__":
    app.run()





# import connexion
# import os
# import sys

# sys.path.insert(0, f"{os.getcwd()}")


# appl = connexion.AioHttpApp(__name__, specification_dir="openapi/")
# appl.add_api("openapi.yaml", arguments={"title": "teste de carga"})


# async def app():
#     return appl


# if __name__ == "__main__":
#     appl.run(port=8080)


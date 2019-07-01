
import connexion
import sys
import os

sys.path.insert(0, f"{os.getcwd()}")


app = connexion.App(__name__, specification_dir="./swagger/")
app.add_api("swagger.yaml", arguments={"title": "teste de carga"})


if __name__ == "__main__":
    app.run()




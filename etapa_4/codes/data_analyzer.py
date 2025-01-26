import pandas as pd
import matplotlib.pyplot as plt


def main():
    """Função main"""

    data = pd.read_csv("/etapa_4/image_files/string_output1.csv", delimiter=";")

    x_values = data["Index"]
    y_values = data["Time"]

    # Plot data
    plt.figure(figsize=(10, 6))
    plt.plot(x_values, y_values, marker="o", label="Data Points")

    plt.xlabel("Índice")
    plt.ylabel("Tempo")
    plt.title("Análise Temporal")
    plt.grid()
    plt.legend()

    plt.show()


main()

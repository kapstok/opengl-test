import numpy as np
import matplotlib.pyplot as plt

# Het beginpunt van de boog is (start_x+straallengte,0).
# Het eindpunt van de boog bij 90 graden is (start_x,straallengte).

# Arc algorithm
# Functie om alleen de cirkelboog te tekenen en een punt op de boog te markeren
def plot_cirkelboog_met_punt(straallengte, start_x, start_angle, end_angle):
    # Zet de hoeken om in radialen voor de boog
    theta = np.linspace(np.radians(start_angle), np.radians(end_angle), 100)

    # Bereken de coördinaten van de boog vanaf (start_x, 0)
    x_boog = start_x + straallengte * np.cos(theta)
    y_boog = straallengte * np.sin(theta)

    # Bepaal de hoek voor een vijfde van de boog
    hoek_vijfde = start_angle + (end_angle - start_angle) / 5

    # Zet deze hoek om in radialen
    theta_vijfde = np.radians(hoek_vijfde)

    # Bereken de x- en y-coördinaten voor het punt op een vijfde van de boog
    x_punt_vijfde = start_x + straallengte * np.cos(theta_vijfde)
    y_punt_vijfde = straallengte * np.sin(theta_vijfde)

    # Plot de grafiek
    plt.figure(figsize=(6,6))
    
    # Plot de boog
    plt.plot(x_boog, y_boog, label=f"Cirkelboog van {start_angle}° tot {end_angle}°", color='blue')

    # Markeer het punt op een vijfde van de boog
    plt.plot(x_punt_vijfde, y_punt_vijfde, 'ro', label=f'Punt op 1/5 van de boog ({x_punt_vijfde:.2f}, {y_punt_vijfde:.2f})')

    # Instellen van de assen voor gelijke verhoudingen
    plt.gca().set_aspect('equal')
    
    # Labels en legenda toevoegen
    plt.xlabel("X-as")
    plt.ylabel("Y-as")
    plt.legend()
    
    # Titel en tonen van de grafiek
    plt.title("Visualisatie van een cirkelboog met een gemarkeerd punt")
    plt.grid(True)
    plt.show()

# Parameters voor de cirkelboog
straallengte = 3
start_x = 4
start_angle = 0   # Boog start horizontaal bij 0 graden
end_angle = 90    # Boog eindigt bij 90 graden

# Visualiseer alleen de cirkelboog en het punt op een vijfde van de boog
plot_cirkelboog_met_punt(straallengte, start_x, start_angle, end_angle)

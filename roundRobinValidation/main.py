import json
import matplotlib.pyplot as plt
import numpy as np

# Leggi il JSON da un file
file_path = 'result_ok.json'
with open(file_path, 'r') as file:
    json_data = json.load(file)

# Inizializza un dizionario per memorizzare i dati cumulativi e nel tempo per ogni utente
cumulative_data = {}
time_data = {}

# Estrai dati utili
for vector in json_data["vectors"]:
    user_name = vector["module"].replace("Network.", "")  # Rimuovi "Network" dai nomi degli utenti
    packets_received = np.array(vector["value"])

    # Calcola la somma cumulativa
    cumulative_packets = np.sum(packets_received)

    # Aggiungi dati cumulativi al dizionario
    cumulative_data[user_name] = cumulative_packets

    # Aggiungi dati nel tempo al dizionario
    time_data[user_name] = packets_received

# Genera una lista di colori una sola volta per tutti gli utenti
colors = plt.cm.viridis(np.linspace(0, 1, len(cumulative_data)))

# Grafico istogramma con colori diversi per ogni utente
plt.figure(figsize=(12, 6))

# Istogramma
plt.subplot(1, 2, 1)
plt.bar(cumulative_data.keys(), cumulative_data.values(), color=colors, alpha=0.7)
plt.xlabel('User')
plt.ylabel('Cumulative Packets Received')
plt.title('Cumulative Histogram of Packets Received for Each User')

# Grafico nel tempo
plt.subplot(1, 2, 2)
for i, (user_name, time_values) in enumerate(time_data.items()):
    plt.plot(time_values, label=user_name, color=colors[i])

plt.xlabel('Time Slot')
plt.ylabel('Packets Received')
plt.title('Packets Received Over Time for Each User')
plt.legend()

# Mostra i grafici
plt.tight_layout()
plt.show()

import customtkinter
import json
import os


# CONFIGURAÇÕES GERAIS
customtkinter.set_appearance_mode("dark")  # tema escuro
customtkinter.set_default_color_theme("green")  # define um tema base verde

# Caminho da pasta onde os dados serão salvos
PASTA_DADOS = "dados"
ARQUIVO_USUARIOS = os.path.join(PASTA_DADOS, "usuarios.json")

# Cria a pasta "dados" automaticamente, se ainda não existir
if not os.path.exists(PASTA_DADOS):
    os.makedirs(PASTA_DADOS)

# FUNÇÕES DE ARQUIVO
def carregar_usuarios():
    """Carrega o arquivo JSON de usuários."""
    if not os.path.exists(ARQUIVO_USUARIOS):
        return {}
    with open(ARQUIVO_USUARIOS, "r") as f:
        return json.load(f)

def salvar_usuarios(usuarios):
    """Salva o dicionário de usuários no arquivo JSON."""
    with open(ARQUIVO_USUARIOS, "w") as f:
        json.dump(usuarios, f, indent=4)

# TELA DE LOGIN
def abrir_login():
    """Exibe a tela de login."""
    login = customtkinter.CTk()
    login.geometry("500x350")
    login.title("Tela de Login")

    # Cor de fundo personalizada
    login.configure(fg_color="#3a3a3a")

    def login_usuario():
        """Verifica se o usuário e senha estão corretos."""
        user_email = email.get()
        user_senha = senha.get()
        usuarios = carregar_usuarios()

        if user_email == "" or user_senha == "":
            resultado.configure(text="Preencha todos os campos!", text_color="red")
        elif user_email in usuarios and usuarios[user_email] == user_senha:
            resultado.configure(text=f"Bem-vindo, {user_email}!", text_color="#2ecc71")
        else:
            resultado.configure(text="E-mail ou senha incorretos.", text_color="red")

    def abrir_cadastro():
        """Fecha a tela de login e abre a de cadastro."""
        login.destroy()
        abrir_tela_cadastro()

    # Interface
    #Label = Palavra Fixada
    texto = customtkinter.CTkLabel(login, text="Fazer Login", font=("Arial", 22, "bold"))
    texto.pack(padx=10, pady=10)
    #Entry = Entrada de dados (Tipo ScanF)
    email = customtkinter.CTkEntry(login, placeholder_text="Seu e-mail", width=250)
    email.pack(padx=10, pady=10)

    senha = customtkinter.CTkEntry(login, placeholder_text="Sua senha", show="*", width=250)
    senha.pack(padx=10, pady=10)

    # Botões com cor verde
    botao_login = customtkinter.CTkButton(
        login, text="Login", command=login_usuario, fg_color="#2ecc71", hover_color="#27ae60"
    )
    botao_login.pack(padx=10, pady=10)

    botao_cadastro = customtkinter.CTkButton(
        login, text="Cadastrar", command=abrir_cadastro, fg_color="#27ae60", hover_color="#1e8449"
    )
    botao_cadastro.pack(padx=10, pady=10)

    resultado = customtkinter.CTkLabel(login, text="")
    resultado.pack(padx=10, pady=10)

    login.mainloop()

# TELA DE CADASTRO
def abrir_tela_cadastro():
    """Exibe a tela de cadastro de novo usuário."""
    cadastro = customtkinter.CTk()
    cadastro.geometry("400x300")
    cadastro.title("Cadastrar Usuário")

    # Cor de fundo personalizada
    cadastro.configure(fg_color="#3a3a3a")  # mesmo cinza do login

    texto_cadastro = customtkinter.CTkLabel(cadastro, text="Cadastro de Novo Usuário", font=("Arial", 18, "bold"))
    texto_cadastro.pack(padx=10, pady=10)

    email_cadastro = customtkinter.CTkEntry(cadastro, placeholder_text="Novo e-mail", width=250)
    email_cadastro.pack(padx=10, pady=10)

    senha_cadastro = customtkinter.CTkEntry(cadastro, placeholder_text="Nova senha", show="*", width=250)
    senha_cadastro.pack(padx=10, pady=10)

    resultado_cadastro = customtkinter.CTkLabel(cadastro, text="")
    resultado_cadastro.pack(padx=10, pady=10)

    def cadastrar():
        #Cadastra um novo usuário e salva no arquivo JSON.
        novo_email = email_cadastro.get()
        nova_senha = senha_cadastro.get()

        if novo_email == "" or nova_senha == "":
            resultado_cadastro.configure(text="Preencha todos os campos!", text_color="red")
            return

        usuarios = carregar_usuarios()

        if novo_email in usuarios:
            resultado_cadastro.configure(text="Usuário já cadastrado!", text_color="orange")
        else:
            usuarios[novo_email] = nova_senha
            salvar_usuarios(usuarios)
            resultado_cadastro.configure(text="Usuário cadastrado com sucesso!", text_color="#2ecc71")

            # Fecha o cadastro e volta ao login
            cadastro.after(1500, lambda: (cadastro.destroy(), abrir_login()))

    botao_cadastrar = customtkinter.CTkButton(
        cadastro, text="Cadastrar", command=cadastrar, fg_color="#2ecc71", hover_color="#27ae60"
    )
    botao_cadastrar.pack(padx=10, pady=10)

    botao_voltar = customtkinter.CTkButton(
        cadastro, text="Voltar", command=lambda: (cadastro.destroy(), abrir_login()), fg_color="#27ae60", hover_color="#1e8449"
    )
    botao_voltar.pack(padx=10, pady=10)

    cadastro.mainloop()

# PONTO DE ENTRADA
abrir_login()

defmodule Mix.Tasks.Compile.Nif do
  def run(_args) do
    build_dir =
      Mix.Project.config()
      |> Mix.Project.app_path()

    IO.binwrite "Building TidesDB C library..."
    {result, _} = System.cmd("make", [], env: [{"BUILD_DIR", build_dir}], stderr_to_stdout: true)
    IO.binwrite(result)
  end
end

defmodule TidesDB.MixProject do
  use Mix.Project

  def project do
    [
      app: :tidesdb,
      version: "0.1.0",
      elixir: "~> 1.18",
      compilers: [:nif] ++ Mix.compilers,
      start_permanent: Mix.env() == :prod,
      deps: deps()
    ]
  end

  def application do
    [
      extra_applications: [:logger]
    ]
  end

  defp deps do
    []
  end
end

defmodule TidesDB.NIF do
  @moduledoc false

  @on_load :load_nif

  def load_nif do
    Application.app_dir(:tidesdb, ["priv", "tidesdb"]) |> :erlang.load_nif(0)
  end

  def open(_path, _config), do: :erlang.nif_error(:nif_not_loaded)
  def close(_db), do: :erlang.nif_error(:nif_not_loaded)

  def create_column_family(_db, _name, _config), do: :erlang.nif_error(:nif_not_loaded)
  def get_column_family(_db, _name), do: :erlang.nif_error(:nif_not_loaded)
  def list_column_families(_db), do: :erlang.nif_error(:nif_not_loaded)

  def txn_begin(_db), do: :erlang.nif_error(:nif_not_loaded)
  def txn_put(_txn, _cf, _key, _value), do: :erlang.nif_error(:nif_not_loaded)
  def txn_get(_txn, _cf, _key), do: :erlang.nif_error(:nif_not_loaded)
  def txn_delete(_txn, _cf, _key), do: :erlang.nif_error(:nif_not_loaded)
  def txn_rollback(_txn), do: :erlang.nif_error(:nif_not_loaded)
  def txn_commit(_txn), do: :erlang.nif_error(:nif_not_loaded)

  def iter_new(_txn, _cf), do: :erlang.nif_error(:nif_not_loaded)
  def iter_close(_iter), do: :erlang.nif_error(:nif_not_loaded)
  def iter_valid(_iter), do: :erlang.nif_error(:nif_not_loaded)
  def iter_seek_to_first(_iter), do: :erlang.nif_error(:nif_not_loaded)
  def iter_seek_to_last(_iter), do: :erlang.nif_error(:nif_not_loaded)
  def iter_key(_iter), do: :erlang.nif_error(:nif_not_loaded)
  def iter_value(_iter), do: :erlang.nif_error(:nif_not_loaded)
  def iter_next(_iter), do: :erlang.nif_error(:nif_not_loaded)
  def iter_prev(_iter), do: :erlang.nif_error(:nif_not_loaded)

  def scan(_db, _cf, _num), do: :erlang.nif_error(:nif_not_loaded)
end

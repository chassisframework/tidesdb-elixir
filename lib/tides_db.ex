defmodule TidesDB do
  @moduledoc """
  Documentation for `TidesDB`.
  """

  @doc """
  """

  alias TidesDB.NIF
  alias TidesDB.DB
  alias TidesDB.ColumnFamily
  alias TidesDB.Transaction
  alias TidesDB.Iterator

  def open(path, config \\ []) when is_binary(path) and path != "" and is_list(config) do
    name = String.to_charlist(path)

    case NIF.open(name, config) do
      {:ok, ref} ->
        {:ok, %DB{ref: ref}}

      error ->
        error
    end
  end

  def close(%DB{} = db) do
    NIF.close(db.ref)
  end

  def create_column_family(%DB{} = db, name, config \\ []) when is_binary(name) and name != "" and is_list(config) do
    name = String.to_charlist(name)

    NIF.create_column_family(db.ref, name, config)
  end

  def get_column_family(%DB{} = db, name) when is_binary(name) and name != "" do
    name = String.to_charlist(name)

    case NIF.get_column_family(db.ref, name) do
      {:ok, ref} ->
        {:ok, %ColumnFamily{ref: ref}}

      error ->
        error
    end
  end

  def list_column_families(%DB{} = db) do
    case NIF.list_column_families(db.ref) do
      {:ok, names} ->
        {:ok, Enum.map(names, &:erlang.list_to_binary/1)}

      error ->
        error
    end
  end

  def txn_begin(%DB{} = db) do
    case NIF.txn_begin(db.ref) do
      {:ok, ref} ->
        {:ok, %Transaction{ref: ref}}

      error ->
        error
    end
  end

  def txn_put(%Transaction{} = txn, %ColumnFamily{} = cf, key, value) when is_binary(key) and is_binary(value) do
    NIF.txn_put(txn.ref, cf.ref, key, value)
  end

  def txn_get(%Transaction{} = txn, %ColumnFamily{} = cf, key) when is_binary(key) do
    NIF.txn_get(txn.ref, cf.ref, key)
  end

  def txn_delete(%Transaction{} = txn, %ColumnFamily{} = cf, key) when is_binary(key) do
    NIF.txn_delete(txn.ref, cf.ref, key)
  end

  def txn_rollback(%Transaction{} = txn) do
    NIF.txn_rollback(txn.ref)
  end

  def txn_commit(%Transaction{} = txn) do
    NIF.txn_commit(txn.ref)
  end

  def iter_new(%Transaction{} = txn, %ColumnFamily{} = cf) do
    case NIF.iter_new(txn.ref, cf.ref) do
      {:ok, ref} ->
        {:ok, %Iterator{ref: ref}}

      error ->
        error
    end
  end

  def iter_close(%Iterator{} = iter) do
    NIF.iter_close(iter.ref)
  end

  def iter_valid?(%Iterator{} = iter) do
    NIF.iter_valid(iter.ref)
  end

  def iter_seek_to_first(%Iterator{} = iter) do
    NIF.iter_seek_to_first(iter.ref)
  end

  def iter_seek_to_last(%Iterator{} = iter) do
    NIF.iter_seek_to_last(iter.ref)
  end

  def iter_key(%Iterator{} = iter) do
    NIF.iter_key(iter.ref)
  end

  def iter_value(%Iterator{} = iter) do
    NIF.iter_value(iter.ref)
  end

  def iter_next(%Iterator{} = iter) do
    NIF.iter_next(iter.ref)
  end

  def iter_prev(%Iterator{} = iter) do
    NIF.iter_prev(iter.ref)
  end

  def scan(db, cf, num \\ nil)
  def scan(%DB{}, %ColumnFamily{}, 0), do: []
  def scan(%DB{} = db, %ColumnFamily{} = cf, nil), do: scan(db, cf, -1)
  def scan(%DB{} = db, %ColumnFamily{} = cf, num) when is_integer(num) do
    NIF.scan(db.ref, cf.ref, num)
  end
end

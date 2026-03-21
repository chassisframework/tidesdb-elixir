defmodule TidesDBTest do
  use ExUnit.Case
  doctest TidesDB

  test "smoke test" do
    File.mkdir("dbs")
    name = :crypto.strong_rand_bytes(10) |> Base.encode16()
    path = Path.join("dbs", name)

    assert {:ok, db} = TidesDB.open(path)
    assert :ok = TidesDB.create_column_family(db, "butts")
    assert {:ok, cf} = TidesDB.get_column_family(db, "butts")
    assert {:ok, ["butts"]} = TidesDB.list_column_families(db)

    for i <- 1..2_000 do
      assert {:ok, txn} = TidesDB.txn_begin(db)
      for j <- 1..1_000 do
        assert :ok = TidesDB.txn_put(txn, cf, "some key #{i*j}", "some value #{i*j}")
      end
      assert :ok = TidesDB.txn_commit(txn)
    end

    {t, _v} = :timer.tc fn ->
      TidesDB.scan(db, cf)
    end, :millisecond

    IO.inspect 2_000_000 / t

    {t, _v} = :timer.tc fn ->
      {:ok, txn} = TidesDB.txn_begin(db)
      {:ok, iter} = TidesDB.iter_new(txn, cf)
      :ok = TidesDB.iter_seek_to_first(iter)

      Stream.repeatedly(fn ->
        if TidesDB.iter_valid?(iter) do
          {:ok, key} = TidesDB.iter_key(iter)
          {:ok, value} = TidesDB.iter_value(iter)
          {key, value}
        else
          :eof
        end
      end)
      |> Stream.take_while(fn
        :eof ->
          :ok = TidesDB.iter_close(iter)
          false

        _ ->
          TidesDB.iter_next(iter)
          true
      end)
      |> Enum.into([])
      # |> IO.inspect
      # :ok = TidesDB.txn_commit(txn)
    end, :millisecond

    IO.inspect 2_000_000 / t

    assert {:ok, txn} = TidesDB.txn_begin(db)
    assert :ok = TidesDB.txn_put(txn, cf, "delete me", "delete me value")
    assert {:ok, "delete me value"} = TidesDB.txn_get(txn, cf, "delete me")
    assert :ok = TidesDB.txn_commit(txn)

    assert {:ok, txn} = TidesDB.txn_begin(db)
    assert :ok = TidesDB.txn_delete(txn, cf, "delete me")
    assert {:error, :not_found} = TidesDB.txn_get(txn, cf, "delete me")
    assert :ok = TidesDB.txn_commit(txn)

    assert {:ok, txn} = TidesDB.txn_begin(db)
    assert :ok = TidesDB.txn_put(txn, cf, "rollback", "rollback value")
    assert {:ok, "rollback value"} = TidesDB.txn_get(txn, cf, "rollback")
    assert :ok = TidesDB.txn_rollback(txn)

    assert {:ok, txn} = TidesDB.txn_begin(db)
    assert {:error, :not_found} = TidesDB.txn_get(txn, cf, "rollback")
    assert :ok = TidesDB.txn_commit(txn)

    assert :ok = TidesDB.close(db)
  end
end

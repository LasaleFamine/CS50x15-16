<form class="form-inline" action="sell.php" method="post">
    <fieldset>
        <div class="form-group">
            <select name="symbol" class="form-control" id="symbol">
                <option disabled selected>Symbol</option>
                <?php foreach ($shares as $share): ?>
                    <option value="<?= $share["symbol"] ?>"> <?= $share["symbol"] ?> </option>
                <?php endforeach ?>
            </select>
        </div>
        <div class="form-group">
            <input class="form-control" name="sharesN" placeholder="Number shares" type="text"/>
        </div>
        <div class="form-group">
            <button class="btn btn-default" type="submit">
                <span aria-hidden="true" class="glyphicon glyphicon-log-in"></span>
                Sell
            </button>
        </div>
    </fieldset>
</form>